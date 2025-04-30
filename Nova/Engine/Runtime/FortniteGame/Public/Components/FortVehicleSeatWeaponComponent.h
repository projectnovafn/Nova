#pragma once

#include "Components/ActorComponent.h"

class UFortWeaponItemDefinition;
class AFortPawn;

class FWeaponSeatDefinition
{
	GENERATED_USTRUCT_BODY(FWeaponSeatDefinition, FortniteGame)

public:

	UPROPERTY_STRUCT(int, SeatIndex)
	int SeatIndex;

	UPROPERTY_STRUCT(UFortWeaponItemDefinition*, VehicleWeapon)
	UFortWeaponItemDefinition* VehicleWeapon;

	UPROPERTY_STRUCT(UFortWeaponItemDefinition*, VehicleWeaponOverride)
	UFortWeaponItemDefinition* VehicleWeaponOverride;

	UPROPERTY_STRUCT(UFortWeaponItemDefinition*, LastEquippedVehicleWeapon)
	UFortWeaponItemDefinition* LastEquippedVehicleWeapon;
};

class UFortVehicleSeatWeaponComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY(UFortVehicleSeatWeaponComponent, FortniteGame)

public:

	UPROPERTY(TArray<FWeaponSeatDefinition>, WeaponSeatDefinitions)
	TArray<FWeaponSeatDefinition> WeaponSeatDefinitions;

	UPROPERTY(int32, ActiveSeatIdx)
	int32 ActiveSeatIdx;
};