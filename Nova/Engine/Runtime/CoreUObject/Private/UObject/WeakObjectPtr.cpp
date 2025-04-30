#include "UObject/WeakObjectPtr.h"

FWeakObjectPtr::FWeakObjectPtr(UObject* Value)
{
	static auto FWeakObjectPtrCtor = UKismetMemoryLibrary::Get<void (*)(FWeakObjectPtr*, UObject*)>(L"FWeakObjectPtr::FWeakObjectPtr");

	FWeakObjectPtrCtor(this, Value);
}

void FWeakObjectPtr::Reset()
{
	ObjectIndex = -1;
	ObjectSerialNumber = 0;
}

UObject* FWeakObjectPtr::Get()
{
	static auto Get = UKismetMemoryLibrary::Get<UObject* (*)(FWeakObjectPtr*)>(L"FWeakObjectPtr::Get");

	return Get(this);
}