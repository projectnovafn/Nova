#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakObjectPtr.h"

/**
 * TPersistentObjectPtr is a template base class for FLazyObjectPtr and FSoftObjectPtr
 */
template<class TObjectID>
struct TPersistentObjectPtr
{
	FWeakObjectPtr WeakPtr;
	int32 TagAtLastTest;
	TObjectID ObjectID;
};

template <class TObjectID> struct TIsPODType<TPersistentObjectPtr<TObjectID> > { enum { Value = TIsPODType<TObjectID>::Value }; };