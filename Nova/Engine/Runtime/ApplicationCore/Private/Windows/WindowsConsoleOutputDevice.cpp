#include "Windows/WindowsConsoleOutputDevice.h"
#include "Logging/LogMacros.h"
#include "Misc/OutputDevice.h"
#include "HAL/UnrealMemory.h"
#include "Templates/UnrealTemplate.h"
#include "CoreGlobals.h"
#include "Misc/CString.h"

#include <Windows.h>
#include <iostream>
#include <stdio.h>

#include "HAL/PlatformTime.h"
#include "Misc/OutputDeviceHelper.h"

namespace OutputDeviceConstants
{
	uint32 WIN_STD_OUTPUT_HANDLE = STD_OUTPUT_HANDLE;
	uint32 WIN_ATTACH_PARENT_PROCESS = ATTACH_PARENT_PROCESS;
}

FWindowsConsoleOutputDevice::FWindowsConsoleOutputDevice()
	: ConsoleHandle(0)
	, OverrideColorSet(false)
	, bIsAttached(false)
{
}

void FWindowsConsoleOutputDevice::Show(bool ShowWindow)
{
	if (ShowWindow)
	{
		if (!ConsoleHandle)
		{
			if (!AllocConsole())
			{
				bIsAttached = true;
			}

			ConsoleHandle = GetStdHandle(OutputDeviceConstants::WIN_STD_OUTPUT_HANDLE);

			if (ConsoleHandle != INVALID_HANDLE_VALUE)
			{
				COORD Size;
				Size.X = 160;
				Size.Y = 4000;

				int32 ConsoleWidth = 160;
				int32 ConsoleHeight = 4000;
				int32 ConsolePosX = 0;
				int32 ConsolePosY = 0;
				bool bHasX = false;
				bool bHasY = false;

				SetConsoleScreenBufferSize(ConsoleHandle, Size);

				CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;

				if (::GetConsoleScreenBufferInfo(ConsoleHandle, &ConsoleInfo) != 0)
				{
					SMALL_RECT NewConsoleWindowRect = ConsoleInfo.srWindow;
					NewConsoleWindowRect.Right = ConsoleInfo.dwSize.X - 1;
					::SetConsoleWindowInfo(ConsoleHandle, true, &NewConsoleWindowRect);
				}
			}
		}
	}
	else if (ConsoleHandle)
	{
		ConsoleHandle = NULL;
		FreeConsole();
		bIsAttached = false;
	}
}

bool FWindowsConsoleOutputDevice::IsShown()
{
	return ConsoleHandle != NULL;
}

void FWindowsConsoleOutputDevice::SetColor(const TCHAR* Color)
{
	if (FCString::Stricmp(Color, TEXT("")) == 0)
	{
		SetConsoleTextAttribute(ConsoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	}
	else
	{
		TCHAR String[9];
		FMemory::Memset(String, 0, sizeof(TCHAR) * 9);
		FCString::Strncpy(String, Color, 9);
		for (TCHAR* S = String; *S; S++)
		{
			*S -= '0';
		}

		SetConsoleTextAttribute(ConsoleHandle,
			(String[0] ? FOREGROUND_RED : 0) |
			(String[1] ? FOREGROUND_GREEN : 0) |
			(String[2] ? FOREGROUND_BLUE : 0) |
			(String[3] ? FOREGROUND_INTENSITY : 0) |
			(String[4] ? BACKGROUND_RED : 0) |
			(String[5] ? BACKGROUND_GREEN : 0) |
			(String[6] ? BACKGROUND_BLUE : 0) |
			(String[7] ? BACKGROUND_INTENSITY : 0));
	}
}

bool FWindowsConsoleOutputDevice::IsAttached()
{
	if (ConsoleHandle != NULL)
	{
		return bIsAttached;
	}
	else if (!AttachConsole(OutputDeviceConstants::WIN_ATTACH_PARENT_PROCESS))
	{
		if (GetLastError() == ERROR_ACCESS_DENIED)
		{
			return true;
		}
	}
	else
	{
		FreeConsole();
	}
	return false;
}

bool FWindowsConsoleOutputDevice::CanBeUsedOnAnyThread() const
{
	return true;
}

bool FWindowsConsoleOutputDevice::CanBeUsedOnPanicThread() const
{
	return true;
}

void FWindowsConsoleOutputDevice::Serialize(const TCHAR* Data, ELogVerbosity::Type Verbosity, const TCHAR* Category, const double Time)
{
	if (ConsoleHandle)
	{
		const double RealTime = Time == -1.0f ? FPlatformTime::Seconds() : Time;

		static bool Entry = false;
		if (!Entry)
		{
			if (Verbosity == ELogVerbosity::SetColor)
			{
				SetColor(Data);
				OverrideColorSet = FCString::Strcmp(COLOR_NONE, Data) != 0;
			}
			else
			{
				bool bNeedToResetColor = false;
				if (!OverrideColorSet)
				{
					if (Verbosity == ELogVerbosity::Error)
					{
						SetColor(COLOR_RED);
						bNeedToResetColor = true;
					}
					else if (Verbosity == ELogVerbosity::Warning)
					{
						SetColor(COLOR_YELLOW);
						bNeedToResetColor = true;
					}
				}

				TCHAR OutputString[1024]{};
				_stprintf_s(OutputString, 1024, TEXT("%s\r\n"), *FOutputDeviceHelper::FormatLogLine(Verbosity, Category, Data, ELogTimes::Type::UTC, RealTime));

				uint32 Written;
				WriteConsole(ConsoleHandle, OutputString, FCString::Strlen(OutputString), (::DWORD*)&Written, NULL);

				if (bNeedToResetColor)
				{
					SetColor(COLOR_NONE);
				}
			}
		}
		else
		{
			Entry = true;
			Serialize(Data, Verbosity, Category);
			Entry = false;
		}
	}
}

void FWindowsConsoleOutputDevice::Serialize(const TCHAR* Data, ELogVerbosity::Type Verbosity, const TCHAR* Category)
{
	Serialize(Data, Verbosity, Category, -1.0);
}

void FWindowsConsoleOutputDevice::LogInternal(const TCHAR* Message, va_list args)
{
	TCHAR Buffer[1024]{};
	_vstprintf_s(Buffer, 1024, Message, args);

	if (IsShown())
	{
		DWORD BytesWritten;
		WriteConsoleW(this->ConsoleHandle, Buffer, _tcslen(Buffer), &BytesWritten, nullptr);
	}
}