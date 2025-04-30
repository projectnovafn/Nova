#include "Components/SceneComponent.h"

void USceneComponent::SetWorldTransform(FTransform NewTransform, bool bSweep, bool bTeleport)
{
	struct
	{
		FTransform NewTransform; 
		bool bSweep; 
		bool bTeleport; 
		FHitResult HitResult;
	} params(NewTransform, bSweep, bTeleport);

	this->ProcessEvent(L"K2_SetWorldTransform", &params);
}

void USceneComponent::SetPhysicsLinearVelocity(FVector NewVel, bool bAddToCurrent, FName BoneName)
{
	struct
	{
		FVector NewVel;
		bool bAddToCurrent;
		FName BoneName;
	} params(NewVel, bAddToCurrent, BoneName);

	this->ProcessEvent(L"SetPhysicsLinearVelocity", &params);
}

void USceneComponent::SetPhysicsAngularVelocityInRadians(FVector NewAngVel, bool bAddToCurrent, FName BoneName)
{
	struct
	{
		FVector NewAngVel;
		bool bAddToCurrent;
		FName BoneName;
	} params(NewAngVel, bAddToCurrent, BoneName);

	this->ProcessEvent(L"SetPhysicsAngularVelocityInRadians", &params);
}

FVector USceneComponent::GetComponentLocation()
{
	FVector ReturnValue;
	this->ProcessEvent(L"K2_GetComponentLocation", &ReturnValue);

	return ReturnValue;
}

FTransform USceneComponent::GetComponentToWorld()
{
	FTransform ReturnValue;
	this->ProcessEvent(L"K2_GetComponentToWorld", &ReturnValue);
	
	return ReturnValue;
}