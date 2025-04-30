#include "GenericPlatform/GenericPlatformMisc.h"

#include <iostream>

void FGenericPlatformMisc::RequestExit(bool Force, const TCHAR* CallSite)
{
}

void FGenericPlatformMisc::RequestExitWithStatus(bool Force, uint8 ReturnCode, const TCHAR* CallSite)
{
}

void FGenericPlatformMisc::ResumeMainThread()
{
}

bool FGenericPlatformMisc::IsWithinImage(void* Target)
{
	return true;
}

bool FGenericPlatformMisc::IsDebuggerPresent()
{
	return false;
}

void FGenericPlatformMisc::PauseForDebugger()
{
}
