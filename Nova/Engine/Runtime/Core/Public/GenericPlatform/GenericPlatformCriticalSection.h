#pragma once

#include "CoreTypes.h"

template<class CriticalSection>
class TGenericPlatformRWLock
{
public:
	FORCEINLINE TGenericPlatformRWLock()
	{
	}

	FORCEINLINE ~TGenericPlatformRWLock()
	{
	}

	FORCEINLINE void ReadLock()
	{
		Mutex.Lock();
	}

	FORCEINLINE void WriteLock()
	{
		Mutex.Lock();
	}

	FORCEINLINE void ReadUnlock()
	{
		Mutex.Unlock();
	}

	FORCEINLINE void WriteUnlock()
	{
		Mutex.Unlock();
	}

private:
	CriticalSection Mutex;
};

#if PLATFORM_WINDOWS
#include "Windows\WindowsCriticalSection.h"
#endif