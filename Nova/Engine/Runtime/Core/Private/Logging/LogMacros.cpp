#include "Logging/LogMacros.h"
#include "CoreGlobals.h"
#include "HAL/Platform.h"

void FMsg::LogfImpl(const ANSICHAR* File, int32 Line, const FLogCategoryName& Category, ELogVerbosity::Type Verbosity, const TCHAR* Fmt, ...)
{
#if !NO_LOGGING
	if (LIKELY(Verbosity != ELogVerbosity::Fatal))
	{
		va_list Args;
		va_start(Args, Fmt);

		wchar_t Buffer[8096];
		vswprintf_s(Buffer, 8096, Fmt, Args);

		wprintf(Buffer);

		va_end(Args);		
	}
#endif
}

void FMsg::LogV(const ANSICHAR* File, int32 Line, const FLogCategoryName& Category, ELogVerbosity::Type Verbosity, const TCHAR* Fmt, va_list Args)
{
#if !NO_LOGGING
	if (LIKELY(Verbosity != ELogVerbosity::Fatal))
	{
		wchar_t Buffer[8096];
		vswprintf_s(Buffer, 8096, Fmt, Args);

		GLogConsole->Serialize(Buffer, Verbosity, Category);
	}
#endif
}

void FMsg::Logf_InternalImpl(const ANSICHAR* File, int32 Line, const FLogCategoryName& Category, ELogVerbosity::Type Verbosity, const TCHAR* Fmt, ...)
{
#if !NO_LOGGING
	va_list Args;
	va_start(Args, Fmt);
	LogV(File, Line, Category, Verbosity, Fmt, Args);
	va_end(Args);
#endif
}

void VARARGS FMsg::SendNotificationStringfImpl(const TCHAR* Fmt, ...)
{
	//GROWABLE_LOGF(SendNotificationString(Buffer));
}

void FMsg::SendNotificationString(const TCHAR* Message)
{
	//FPlatformMisc::LowLevelOutputDebugString(Message);
}