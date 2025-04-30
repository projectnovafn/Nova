#include "Async/Async.h"

#include "Core.h"

FCriticalSection TasksMutex;

TMap<ENamedThreads::Type, TArray<FRunnable*>> Tasks;

void InternalAsyncTask(ENamedThreads::Type Thread, FRunnable* Callable)
{
	TasksMutex.Lock();

	if (!Tasks.Contains(Thread))
		Tasks.Add(Thread, {});

	Tasks[Thread].Add(Callable);

	TasksMutex.Unlock();
}

void InternalQueueTask(ENamedThreads::Type Thread, float Seconds, FRunnable* Callable)
{
	AsyncThread([Thread, Seconds, Callable]()
	{
		FPlatformProcess::Sleep(Seconds);

		AsyncTask(Thread, [Callable]() { Callable->Run(); });
	});
}

void RunTaskQueue(ENamedThreads::Type Thread)
{
	TArray<FRunnable*> TasksToRun;

	TasksMutex.Lock();

	if (Tasks.Contains(Thread))
	{
		for (auto& Task : Tasks[Thread])
			TasksToRun.Add(Task);

		Tasks[Thread] = {};
	}

	TasksMutex.Unlock();

	for (auto& Task : TasksToRun)
		Task->Run();
}