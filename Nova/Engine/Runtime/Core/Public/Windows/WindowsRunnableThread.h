#pragma once

#include "CoreTypes.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

#include <windows.h>

class FRunnable;

/**
 * This is the base interface for all runnable thread classes. It specifies the
 * methods used in managing its life cycle.
 */
class FRunnableThreadWin
	: public FRunnableThread
{
	/** The thread handle for the thread. */
	HANDLE Thread = 0;

	/**
	 * The thread entry point. Simply forwards the call on to the right
	 * thread main function
	 */
	static ::DWORD STDCALL _ThreadProc(LPVOID pThis)
	{
		auto* ThisThread = (FRunnableThreadWin*)pThis;
		return ThisThread->GuardedRun();
	}

	/** Guarding works only if debugger is not attached or GAlwaysReportCrash is true. */
	uint32 GuardedRun();

	/**
	 * The real thread entry point. It calls the Init/Run/Exit methods on
	 * the runnable object
	 */
	uint32 Run();

public:
	~FRunnableThreadWin()
	{
		if (Thread)
		{
			Kill(true);
		}
	}

	virtual void Suspend(bool bShouldPause = true) override
	{
		if (bShouldPause == true)
		{
			SuspendThread(Thread);
		}
		else
		{
			ResumeThread(Thread);
		}
	}

	virtual bool Kill(bool bShouldWait = false) override
	{
		bool bDidExitOK = true;

		if (Runnable)
		{
			Runnable->Stop();
		}

		if (bShouldWait == true)
		{
			WaitForSingleObject(Thread, INFINITE);
		}

		CloseHandle(Thread);
		Thread = NULL;

		return bDidExitOK;
	}

	virtual void WaitForCompletion() override
	{
		WaitForSingleObject(Thread, INFINITE);
	}

protected:

	virtual bool CreateInternal(FRunnable* InRunnable, const TCHAR* InThreadName,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal, uint64 InThreadAffinityMask = 0,
		EThreadCreateFlags InCreateFlags = EThreadCreateFlags::None) override
	{
		static bool bOnce = false;
		if (!bOnce)
		{
			bOnce = true;
		}

		Runnable = InRunnable;
		ThreadAffinityMask = InThreadAffinityMask;
		ThreadPriority = InThreadPri;

		Thread = CreateThread(NULL, InStackSize, _ThreadProc, this, STACK_SIZE_PARAM_IS_A_RESERVATION | CREATE_SUSPENDED, (::DWORD*)&ThreadID);

		if (Thread == NULL)
		{
			Runnable = nullptr;
		}
		else
		{
			ResumeThread(Thread);
			ThreadPriority = TPri_Normal;
		}

		return Thread != NULL;
	}
};