#pragma once
#include "ActorComponent.h"
#include "Engine/EngineTypes.h"

class USceneComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY(USceneComponent, Engine)
public:

	void SetWorldTransform(FTransform NewTransform, bool bSweep, bool bTeleport);

	void SetPhysicsLinearVelocity(FVector NewVel, bool bAddToCurrent, FName BoneName);
	void SetPhysicsAngularVelocityInRadians(FVector NewAngVel, bool bAddToCurrent, FName BoneName);

	FVector GetComponentLocation();
	FTransform GetComponentToWorld();
};