#pragma once

#include "CoreTypes.h"
#include "GenericPlatform/GenericPlatformTime.h"
#include "WindowsSystemIncludes.h"

struct CORE_API FWindowsPlatformTime
	: public FGenericPlatformTime
{

	static FORCEINLINE double Seconds()
	{
		Windows::LARGE_INTEGER Cycles;
		Windows::QueryPerformanceCounter(&Cycles);

		return Cycles.QuadPart * GetSecondsPerCycle() + 16777216.0;
	}

	static FORCEINLINE uint32 Cycles()
	{
		Windows::LARGE_INTEGER Cycles;
		Windows::QueryPerformanceCounter(&Cycles);
		return Cycles.QuadPart;
	}

	static FORCEINLINE uint64 Cycles64()
	{
		Windows::LARGE_INTEGER Cycles;
		QueryPerformanceCounter(&Cycles);
		return Cycles.QuadPart;
	}


	static void SystemTime(int32& Year, int32& Month, int32& DayOfWeek, int32& Day, int32& Hour, int32& Min, int32& Sec, int32& MSec);
	static void UtcTime(int32& Year, int32& Month, int32& DayOfWeek, int32& Day, int32& Hour, int32& Min, int32& Sec, int32& MSec);
};


typedef FWindowsPlatformTime FPlatformTime;