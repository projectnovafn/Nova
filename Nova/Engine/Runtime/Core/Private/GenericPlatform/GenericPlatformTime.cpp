#include "GenericPlatform/GenericPlatformTime.h"
#include "Logging/LogMacros.h"
#include "HAL/PlatformTime.h"

double FGenericPlatformTime::SecondsPerCycle = 0.0;
double FGenericPlatformTime::SecondsPerCycle64 = 0.0;
double FGenericPlatformTime::LastIntervalCPUTimeInSeconds = 0.0;

double FGenericPlatformTime::GetSecondsPerCycle64()
{
	check(SecondsPerCycle64 != 0.0);
	return SecondsPerCycle64;
}

TCHAR* FGenericPlatformTime::StrDate(TCHAR* Dest, SIZE_T DestSize)
{
	int32 Year;
	int32 Month;
	int32 DayOfWeek;
	int32 Day;
	int32 Hour;
	int32 Min;
	int32 Sec;
	int32 MSec;

	FPlatformTime::SystemTime(Year, Month, DayOfWeek, Day, Hour, Min, Sec, MSec);
	swprintf_s(Dest, (int32)DestSize, TEXT("%02d/%02d/%02d"), Month, Day, Year % 100);
	return Dest;
}

TCHAR* FGenericPlatformTime::StrTime(TCHAR* Dest, SIZE_T DestSize)
{
	int32 Year;
	int32 Month;
	int32 DayOfWeek;
	int32 Day;
	int32 Hour;
	int32 Min;
	int32 Sec;
	int32 MSec;

	FPlatformTime::SystemTime(Year, Month, DayOfWeek, Day, Hour, Min, Sec, MSec);
	swprintf_s(Dest, (int32)DestSize, TEXT("%02d:%02d:%02d"), Hour, Min, Sec);
	return Dest;
}

const TCHAR* FGenericPlatformTime::StrTimestamp()
{
	static thread_local TCHAR Result[1024];
	*Result = TEXT('\0');
	StrDate(Result, wcslen(Result));
	FCString::Strcat(Result, TEXT(" "));
	StrTime(Result + FCString::Strlen(Result), wcslen(Result) - FCString::Strlen(Result));
	return Result;
}