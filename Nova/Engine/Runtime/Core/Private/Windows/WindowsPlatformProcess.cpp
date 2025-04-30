#include "Windows\WindowsPlatformProcess.h"
#include "Windows\WindowsRunnableThread.h"

FRunnableThread* FWindowsPlatformProcess::CreateRunnableThread()
{
	return new FRunnableThreadWin();
}

bool FWindowsPlatformProcess::SupportsMultithreading()
{
	return true;
}

CORE_API void FWindowsPlatformProcess::Sleep(float Seconds)
{
	::Sleep((uint32)(Seconds * 1000.0));
}