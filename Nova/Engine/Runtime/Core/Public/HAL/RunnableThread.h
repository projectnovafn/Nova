#pragma once

#include "CoreTypes.h"
#include "HAL/PlatformAffinity.h"

class FRunnable;
class FTlsAutoCleanup;

/**
 * Interface for runnable threads.
 *
 * This interface specifies the methods used to manage a thread's life cycle.
 */
class FRunnableThread
{
	friend class FThreadSingletonInitializer;
	friend class FTlsAutoCleanup;
	friend class FThreadManager;
	friend class FForkableThread;
	friend class FForkProcessHelper;

	/** Index of TLS slot for FRunnableThread pointer. */
	static CORE_API uint32 RunnableTlsSlot;

public:

	/**
	 * Factory method to create a thread with the specified stack size and thread priority.
	 *
	 * @param InRunnable The runnable object to execute
	 * @param ThreadName Name of the thread
	 * @param InStackSize The size of the stack to create. 0 means use the current thread's stack size
	 * @param InThreadPri Tells the thread whether it needs to adjust its priority or not. Defaults to normal priority
	 * @return The newly created thread or nullptr if it failed
	 */
	static CORE_API FRunnableThread* Create(
		class FRunnable* InRunnable,
		const TCHAR* ThreadName,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal,
		uint64 InThreadAffinityMask = FPlatformAffinity::GetNoAffinityMask(),
		EThreadCreateFlags InCreateFlags = EThreadCreateFlags::None);

	/**
	 * Tells the thread to either pause execution or resume depending on the
	 * passed in value.
	 *
	 * @param bShouldPause Whether to pause the thread (true) or resume (false)
	 */
	virtual void Suspend(bool bShouldPause = true) = 0;

	/**
	 * Tells the thread to exit. If the caller needs to know when the thread has exited, it should use the bShouldWait value.
	 * It's highly recommended not to kill the thread without waiting for it.
	 * Having a thread forcibly destroyed can cause leaks and deadlocks.
	 *
	 * The kill method is calling Stop() on the runnable to kill the thread gracefully.
	 *
	 * @param bShouldWait	If true, the call will wait infinitely for the thread to exit.
	 * @return Always true
	 */
	virtual bool Kill(bool bShouldWait = true) = 0;

	/** Halts the caller until this thread is has completed its work. */
	virtual void WaitForCompletion() = 0;

	/**
	 * Thread ID for this thread
	 *
	 * @return ID that was set by CreateThread
	 * @see GetThreadName
	 */
	const uint32 GetThreadID() const
	{
		return ThreadID;
	}

	/** Returns the runnable's thread priority */
	EThreadPriority GetThreadPriority() const
	{
		return ThreadPriority;
	}

	/** Default constructor. */
	CORE_API FRunnableThread();

	/** Virtual destructor */
	CORE_API virtual ~FRunnableThread();

protected:

	/**
	 * Creates the thread with the specified stack size and thread priority.
	 *
	 * @param InRunnable The runnable object to execute
	 * @param ThreadName Name of the thread
	 * @param InStackSize The size of the stack to create. 0 means use the current thread's stack size
	 * @param InThreadPri Tells the thread whether it needs to adjust its priority or not. Defaults to normal priority
	 * @return True if the thread and all of its initialization was successful, false otherwise
	 */
	virtual bool CreateInternal(FRunnable* InRunnable, const TCHAR* InThreadName,
		uint32 InStackSize = 0,
		EThreadPriority InThreadPri = TPri_Normal, uint64 InThreadAffinityMask = 0,
		EThreadCreateFlags InCreateFlags = EThreadCreateFlags::None) = 0;

	/** The runnable object to execute on this thread. */
	FRunnable* Runnable;

	/** The Affinity to run the thread with. */
	uint64 ThreadAffinityMask;

	/** The priority to run the thread at. */
	EThreadPriority ThreadPriority;

	/** ID set during thread creation. */
	uint32 ThreadID;

private:

	/** Called to setup a newly created RunnableThread */
	static CORE_API void SetupCreatedThread(FRunnableThread*& NewThread, class FRunnable* InRunnable, const TCHAR* ThreadName, uint32 InStackSize, EThreadPriority InThreadPri, uint64 InThreadAffinityMask, EThreadCreateFlags InCreateFlags);

	/** Used by the thread manager to tick threads in single-threaded mode */
	virtual void Tick() {}

	/**
	 * Called after the internal thread is created so it can register debug information
	 */
	CORE_API void PostCreate(EThreadPriority ThreadPriority);
};