#include "HAL\ThreadingBase.h"
#include "GenericPlatform\GenericPlatformProcess.h"

FRunnableThread::FRunnableThread()
{

}

FRunnableThread::~FRunnableThread()
{

}

FRunnableThread* FRunnableThread::Create(
	class FRunnable* InRunnable,
	const TCHAR* ThreadName,
	uint32 InStackSize,
	EThreadPriority InThreadPri,
	uint64 InThreadAffinityMask,
	EThreadCreateFlags InCreateFlags)
{
	bool bCreateRealThread = FPlatformProcess::SupportsMultithreading();

	FRunnableThread* NewThread = nullptr;

	if (bCreateRealThread)
	{
		NewThread = FPlatformProcess::CreateRunnableThread();
	}

	if (NewThread)
	{
		SetupCreatedThread(NewThread, InRunnable, ThreadName, InStackSize, InThreadPri, InThreadAffinityMask, InCreateFlags);
	}

	return NewThread;
}

void FRunnableThread::SetupCreatedThread(FRunnableThread*& NewThread, class FRunnable* InRunnable, const TCHAR* ThreadName, uint32 InStackSize, EThreadPriority InThreadPri, uint64 InThreadAffinityMask, EThreadCreateFlags InCreateFlags)
{
	bool bIsValid = NewThread->CreateInternal(InRunnable, ThreadName, InStackSize, InThreadPri, InThreadAffinityMask, InCreateFlags);

	if (bIsValid)
	{
		NewThread->PostCreate(InThreadPri);
	}
	else
	{
		delete NewThread;
		NewThread = nullptr;
	}
}

void FRunnableThread::PostCreate(EThreadPriority ThreadPriority)
{
}