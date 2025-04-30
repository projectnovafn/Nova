#pragma once

#include "CoreMinimal.h"

struct FGenericPlatformProcess
{
	static CORE_API class FRunnableThread* CreateRunnableThread();
	static CORE_API bool SupportsMultithreading();

	static CORE_API void Sleep(float Seconds);
};

#if PLATFORM_WINDOWS
#include "Windows\WindowsPlatformProcess.h"
#endif