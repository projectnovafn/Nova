#pragma once
#include "Core.h"

class FWeakObjectPtr
{
public:
	int32 ObjectIndex;
	int32 ObjectSerialNumber;

	void Reset();

	UObject* Get();

	FWeakObjectPtr(UObject* Value);
};

template<class T>
class TWeakObjectPtr : private FWeakObjectPtr
{
public:

	TWeakObjectPtr(T* Value) : FWeakObjectPtr(Value) {}

	FORCEINLINE void Reset()
	{
		FWeakObjectPtr::Reset();
	}

	FORCEINLINE T* operator->()
	{
		return (T*)FWeakObjectPtr::Get();
	}

	FORCEINLINE T& operator*()
	{
		return *(T*)FWeakObjectPtr::Get();
	}

	FORCEINLINE bool operator!()
	{
		return FWeakObjectPtr::Get() == NULL;
	}

	FORCEINLINE T* Get()
	{
		return (T*)FWeakObjectPtr::Get();
	}
};