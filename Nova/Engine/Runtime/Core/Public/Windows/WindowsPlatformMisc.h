#pragma once

#include "GenericPlatform/GenericPlatformMisc.h"

struct FWindowsPlatformMisc
	: public FGenericPlatformMisc
{
public:

	static void RequestExit(bool Force, const TCHAR* CallSite = nullptr);
	static void RequestExitWithStatus(bool Force, uint8 ReturnCode, const TCHAR* CallSite = nullptr);

	static void ResumeMainThread();
	static bool IsWithinImage(void* Target);

	static bool IsDebuggerPresent();

	static void PauseForDebugger();
};

typedef FWindowsPlatformMisc FPlatformMisc;