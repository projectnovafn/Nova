#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"

class UDataAsset : public UObject
{
	GENERATED_UCLASS_BODY(UDataAsset, Engine)
public:

	UPROPERTY(UClass*, NativeClass)
	UClass* NativeClass;
};

class UPrimaryDataAsset : public UDataAsset
{
	GENERATED_UCLASS_BODY(UPrimaryDataAsset, Engine)

public:

};