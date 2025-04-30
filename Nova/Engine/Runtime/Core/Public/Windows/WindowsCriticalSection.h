#pragma once

#include "CoreTypes.h"
#include "HAL/PlatformMemory.h"
#include "Microsoft\MinimalWindowsApi.h"

class FString;

class FWindowsCriticalSection
{
public:
	FWindowsCriticalSection(const FWindowsCriticalSection&) = delete;
	FWindowsCriticalSection& operator=(const FWindowsCriticalSection&) = delete;

	FORCEINLINE FWindowsCriticalSection()
	{
		CA_SUPPRESS(28125);
		Windows::InitializeCriticalSection(&CriticalSection);
		Windows::SetCriticalSectionSpinCount(&CriticalSection, 4000);
	}

	FORCEINLINE ~FWindowsCriticalSection()
	{
		Windows::DeleteCriticalSection(&CriticalSection);
	}

	FORCEINLINE void Lock()
	{
		Windows::EnterCriticalSection(&CriticalSection);
	}

	FORCEINLINE bool TryLock()
	{
		if (Windows::TryEnterCriticalSection(&CriticalSection))
		{
			return true;
		}
		return false;
	}

	FORCEINLINE void Unlock()
	{
		Windows::LeaveCriticalSection(&CriticalSection);
	}

private:
	Windows::CRITICAL_SECTION CriticalSection;
};

class FWindowsRWLock
{
public:
	FWindowsRWLock(const FWindowsRWLock&) = delete;
	FWindowsRWLock& operator=(const FWindowsRWLock&) = delete;

	FORCEINLINE FWindowsRWLock(uint32 Level = 0)
	{
		Windows::InitializeSRWLock(&Mutex);
	}

	~FWindowsRWLock()
	{
	}

	FORCEINLINE void ReadLock()
	{
		Windows::AcquireSRWLockShared(&Mutex);
	}

	FORCEINLINE void WriteLock()
	{
		Windows::AcquireSRWLockExclusive(&Mutex);
	}

	FORCEINLINE bool TryReadLock()
	{
		return !!Windows::TryAcquireSRWLockShared(&Mutex);
	}

	FORCEINLINE bool TryWriteLock()
	{
		return !!Windows::TryAcquireSRWLockExclusive(&Mutex);
	}

	FORCEINLINE void ReadUnlock()
	{
		Windows::ReleaseSRWLockShared(&Mutex);
	}

	FORCEINLINE void WriteUnlock()
	{
		Windows::ReleaseSRWLockExclusive(&Mutex);
	}

private:

	bool IsLocked()
	{
		if (Windows::TryAcquireSRWLockExclusive(&Mutex))
		{
			Windows::ReleaseSRWLockExclusive(&Mutex);
			return false;
		}
		else
		{
			return true;
		}
	}

	Windows::SRWLOCK Mutex;
};

typedef FWindowsCriticalSection FCriticalSection;
typedef FWindowsRWLock FRWLock;