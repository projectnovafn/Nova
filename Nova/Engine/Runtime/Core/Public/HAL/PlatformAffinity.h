#pragma once

#include "CoreTypes.h"

#if PLATFORM_WINDOWS
#include <Windows\WindowsPlatformAffinity.h> 
#endif

struct FThreadAffinity
{
	uint64 ThreadAffinityMask = FPlatformAffinity::GetNoAffinityMask();
	uint16 ProcessorGroup = 0;
};