#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformAffinity.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

#include "TaskGraphInterfaces.h"

template<typename CallableType>
class TAsyncRunnable
	: public FRunnable
{
public:

	TAsyncRunnable(CallableType InFunction)
		: Function(InFunction)
	{ }

public:

	virtual uint32 Run() override
	{
		Function();

		return 1;
	}

private:

	CallableType Function;
};

template<typename CallableType>
FRunnableThread* AsyncThread(CallableType&& Callable, uint32 StackSize = 0, EThreadPriority ThreadPri = TPri_Normal)
{
	auto Runnable = new TAsyncRunnable(Callable);

	return FRunnableThread::Create(Runnable, TEXT("Thread"), StackSize, ThreadPri);
}

void InternalAsyncTask(ENamedThreads::Type Thread, FRunnable* Callable);
void InternalQueueTask(ENamedThreads::Type Thread, float Seconds, FRunnable* Callable);

template<typename CallableType>
void AsyncTask(ENamedThreads::Type Thread, CallableType&& Callable)
{
	auto Runnable = new TAsyncRunnable(Callable);

	InternalAsyncTask(Thread, Runnable);
}

template<typename CallableType>
void QueueTask(ENamedThreads::Type Thread, float Seconds, CallableType&& Callable)
{
	auto Runnable = new TAsyncRunnable(Callable);

	InternalQueueTask(Thread, Seconds, Runnable);
}

void RunTaskQueue(ENamedThreads::Type Thread);