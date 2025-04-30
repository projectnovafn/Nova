#pragma once

#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UObject/Class.h"

#include "EngineBaseTypes.h"

enum class ELevelCollectionType : uint8
{
    DynamicSourceLevels,
    DynamicDuplicatedLevels,
    StaticLevels,
    MAX,
};

enum ENetRole
{
    ROLE_None,
    ROLE_SimulatedProxy,
    ROLE_AutonomousProxy,
    ROLE_Authority,
    ROLE_MAX,
};

class FHitResult
{
	GENERATED_USTRUCT_BODY(FHitResult, Engine)

	unsigned char Pad[0x88];
public:
	
	UPROPERTY_STRUCT_BOOL(bBlockingHit)
	bool bBlockingHit;

	UPROPERTY_STRUCT_BOOL(bStartPenetrating)
	bool bStartPenetrating;

	UPROPERTY_STRUCT(int, FaceIndex)
	int FaceIndex;

	UPROPERTY_STRUCT(float, Time)
	float Time;

	UPROPERTY_STRUCT(float, Distance)
	float Distance;

	UPROPERTY_STRUCT(FVector, Location)
	FVector Location;

	UPROPERTY_STRUCT(FVector, ImpactPoint)
	FVector ImpactPoint;

	UPROPERTY_STRUCT(FVector, Normal)
	FVector Normal;

	UPROPERTY_STRUCT(FVector, ImpactNormal)
	FVector ImpactNormal;

	UPROPERTY_STRUCT(FVector, TraceStart)
	FVector TraceStart;

	UPROPERTY_STRUCT(FVector, TraceEnd)
	FVector TraceEnd;

	UPROPERTY_STRUCT(float, PenetrationDepth)
	float PenetrationDepth;

	UPROPERTY_STRUCT(int, Item)
	int Item;

	UPROPERTY_STRUCT(FName, BoneName)
	FName BoneName;

	UPROPERTY_STRUCT(FName, MyBoneName)
	FName MyBoneName;

	UPROPERTY_STRUCT(TWeakObjectPtr<class UPrimitiveComponent>, Component)
	TWeakObjectPtr<class UPrimitiveComponent> Component;
};
