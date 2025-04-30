#pragma once
#include "FortProjectileBase.h"

class FAttachedInfo
{
	GENERATED_USTRUCT_BODY(FAttachedInfo, FortniteGame)
public:

	UPROPERTY_STRUCT(FHitResult, Hit)
	FHitResult Hit;

	UPROPERTY_STRUCT(AActor*, AttachedToActor)
	AActor* AttachedToActor;

	UPROPERTY_STRUCT(FVector, AttachOffset)
	FVector AttachOffset;

	UPROPERTY_STRUCT(FVector, VelocityNormalized)
	FVector VelocityNormalized;

	UPROPERTY_STRUCT(float, NarrowPlacementAgainstVelocityThreshold)
	float NarrowPlacementAgainstVelocityThreshold;

	UPROPERTY_STRUCT(float, StickyOffsetFromPhysicsMesh)
	float StickyOffsetFromPhysicsMesh;

	UPROPERTY_STRUCT(float, StickyOffsetFromBoneCenter)
	float StickyOffsetFromBoneCenter;
};

class AFortAttachableProjectileBase : public AFortProjectileBase
{
	GENERATED_UCLASS_BODY(AFortAttachableProjectileBase, FortniteGame)
public:

	UPROPERTY(FAttachedInfo, AttachedInfo)
	FAttachedInfo AttachedInfo;

	void OnRep_ReplicatedAttachedInfo();
};