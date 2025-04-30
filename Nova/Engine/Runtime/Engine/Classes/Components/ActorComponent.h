#pragma once
#include "CoreUObject.h"

class AActor;

class UActorComponent : public UObject
{
	GENERATED_UCLASS_BODY(UActorComponent, Engine)
public:

	AActor* GetOwner();
};