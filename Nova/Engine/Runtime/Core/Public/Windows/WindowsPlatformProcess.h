#pragma once

#include "CoreTypes.h"
#include "GenericPlatform\GenericPlatformProcess.h"

class FRunnableThread;

struct FWindowsPlatformProcess : public FGenericPlatformProcess
{
	static CORE_API class FRunnableThread* CreateRunnableThread();
	static CORE_API bool SupportsMultithreading();

	static CORE_API void Sleep(float Seconds);
};

typedef FWindowsPlatformProcess FPlatformProcess;