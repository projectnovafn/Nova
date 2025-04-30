#pragma once

#include "CoreFwd.h"
#include "CoreTypes.h"
#include "Math/NumericLimits.h"
#include "Misc/EnumClassFlags.h"

#if PLATFORM_CPU_X86_FAMILY
#include <xmmintrin.h> // _mm_prefetch
#elif PLATFORM_CPU_ARM_FAMILY && defined(_MSC_VER)
#include <intrin.h> // __prefetch
#endif

struct FGenericPlatformMisc
{
public:

	static void RequestExit(bool Force, const TCHAR* CallSite = nullptr);
	static void RequestExitWithStatus(bool Force, uint8 ReturnCode, const TCHAR* CallSite = nullptr);

	static void ResumeMainThread();
	static bool IsWithinImage(void* Target);

	static bool IsDebuggerPresent();

	static void PauseForDebugger();
};