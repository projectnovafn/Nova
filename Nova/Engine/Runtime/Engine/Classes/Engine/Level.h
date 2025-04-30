#pragma once
#include "CoreUObject.h"

class UWorld;

class ULevel : public UObject
{
	GENERATED_UCLASS_BODY(ULevel, Engine)
public:

	UPROPERTY(UWorld*, OwningWorld)
	UWorld* OwningWorld;
};