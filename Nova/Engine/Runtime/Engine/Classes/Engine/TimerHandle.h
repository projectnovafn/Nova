#pragma once
#include "CoreMinimal.h"

struct FTimerHandle
{
	friend class FTimerManager;
	friend struct FTimerHeapOrder;

	FTimerHandle()
		: Handle(0)
	{
	}

	bool IsValid() const
	{
		return Handle != 0;
	}

	void Invalidate()
	{
		Handle = 0;
	}

	bool operator==(const FTimerHandle& Other) const
	{
		return Handle == Other.Handle;
	}

	bool operator!=(const FTimerHandle& Other) const
	{
		return Handle != Other.Handle;
	}

private:
	static constexpr uint32 IndexBits = 24;
	static constexpr uint32 SerialNumberBits = 40;

	static_assert(IndexBits + SerialNumberBits == 64, "The space for the timer index and serial number should total 64 bits");

	static constexpr int32  MaxIndex = (int32)1 << IndexBits;
	static constexpr uint64 MaxSerialNumber = (uint64)1 << SerialNumberBits;

	void SetIndexAndSerialNumber(int32 Index, uint64 SerialNumber)
	{
		check(Index >= 0 && Index < MaxIndex);
		check(SerialNumber < MaxSerialNumber);
		Handle = (SerialNumber << IndexBits) | (uint64)(uint32)Index;
	}

	FORCEINLINE int32 GetIndex() const
	{
		return (int32)(Handle & (uint64)(MaxIndex - 1));
	}

	FORCEINLINE uint64 GetSerialNumber() const
	{
		return Handle >> IndexBits;
	}

	uint64 Handle;

	friend uint32 GetTypeHash(const FTimerHandle& InHandle)
	{
		return GetTypeHash(InHandle.Handle);
	}
};