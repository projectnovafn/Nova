#pragma once

#include "Containers/UnrealString.h"
#include "CoreTypes.h"

struct FCPUTime
{
	FCPUTime(float InCPUTimePct, float InCPUTimePctRelative)
		: CPUTimePct(InCPUTimePct)
		, CPUTimePctRelative(InCPUTimePctRelative)
	{}

	FCPUTime& operator+=(const FCPUTime& Other)
	{
		CPUTimePct += Other.CPUTimePct;
		CPUTimePctRelative += Other.CPUTimePctRelative;
		return *this;
	}

	float CPUTimePct;
	float CPUTimePctRelative;
};


struct FGenericPlatformTime
{
	static TCHAR* StrDate(TCHAR* Dest, SIZE_T DestSize);
	static TCHAR* StrTime(TCHAR* Dest, SIZE_T DestSize);
	static const TCHAR* StrTimestamp();

	static bool UpdateCPUTime(float DeltaTime)
	{
		return false;
	}

	static bool UpdateThreadCPUTime(float = 0.0)
	{
		return false;
	}

	static void AutoUpdateGameThreadCPUTime(double UpdateInterval)
	{

	}

	static FCPUTime GetCPUTime()
	{
		return FCPUTime(0.0f, 0.0f);
	}

	static FCPUTime GetThreadCPUTime()
	{
		return FCPUTime(0.0f, 0.0f);
	}

	static double GetLastIntervalCPUTimeInSeconds()
	{
		return LastIntervalCPUTimeInSeconds;
	}

	static double GetLastIntervalThreadCPUTimeInSeconds()
	{
		return 0.0;
	}

	static double GetSecondsPerCycle()
	{
		return SecondsPerCycle;
	}

	static float ToMilliseconds(const uint32 Cycles)
	{
		return (float)double(SecondsPerCycle * 1000.0 * Cycles);
	}

	static float ToSeconds(const uint32 Cycles)
	{
		return (float)double(SecondsPerCycle * Cycles);
	}

	static double GetSecondsPerCycle64();

	static double ToMilliseconds64(const uint64 Cycles)
	{
		return ToSeconds64(Cycles) * 1000.0;
	}

	static double ToSeconds64(const uint64 Cycles)
	{
		return GetSecondsPerCycle64() * double(Cycles);
	}

protected:

	static double SecondsPerCycle;
	static double SecondsPerCycle64;
	static double LastIntervalCPUTimeInSeconds;
};