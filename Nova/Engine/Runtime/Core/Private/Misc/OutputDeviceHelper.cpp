#include "Misc/OutputDeviceHelper.h"
#include "HAL/PlatformTime.h"
#include "CoreGlobals.h"

#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <chrono>

const TCHAR* FOutputDeviceHelper::VerbosityToString(ELogVerbosity::Type Verbosity)
{
	switch (Verbosity & ELogVerbosity::VerbosityMask)
	{
	case ELogVerbosity::NoLogging:
		return TEXT("NoLogging");
	case ELogVerbosity::Fatal:
		return TEXT("Fatal");
	case ELogVerbosity::Error:
		return TEXT("Error");
	case ELogVerbosity::Warning:
		return TEXT("Warning");
	case ELogVerbosity::Display:
		return TEXT("Display");
	case ELogVerbosity::Log:
		return TEXT("Log");
	case ELogVerbosity::Verbose:
		return TEXT("Verbose");
	case ELogVerbosity::VeryVerbose:
		return TEXT("VeryVerbose");
	}
	return TEXT("UknownVerbosity");
}

std::wstring GetCurrentTimeFormatted() 
{
	auto now = std::chrono::system_clock::now();
	auto nowTime = std::chrono::system_clock::to_time_t(now);

	std::wostringstream oss;
	oss.imbue(std::locale(""));
	oss << std::put_time(std::localtime(&nowTime), L"[%Y.%m.%d-%H.%M.%S:") << std::setfill(L'0') << std::setw(3) << GetTickCount() % 1000 << L"]";
	return oss.str() + L"[  0]";
}

FString FOutputDeviceHelper::FormatLogLine(ELogVerbosity::Type Verbosity, const TCHAR* Category, const TCHAR* Message /*= nullptr*/, ELogTimes::Type LogTime /*= ELogTimes::None*/, const double Time /*= -1.0*/)
{
	std::wstring Format = GetCurrentTimeFormatted();

	Format += Category;
	Format += TEXT(": ");

	if (Verbosity != ELogVerbosity::Log)
	{
		Format += VerbosityToString(Verbosity);
		Format += TEXT(": ");
	}

	if (Message)
	{
		Format += Message;
	}

	return Format.c_str();
}