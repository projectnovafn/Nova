#pragma once
#include "Components/ActorComponent.h"

class UMovementComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY(UMovementComponent, Engine)
public:

	UPROPERTY(FVector, Velocity)
	FVector Velocity;
};