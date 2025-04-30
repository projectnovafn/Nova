#pragma once
#include "GameFramework/Actor.h"
#include "Components/ProjectileMovementComponent.h"
#include "Inventory/FortWorldItem.h"

class AFortPawn;

enum class EFortPickupSpawnSource : uint8
{
	Unset = 0,
	PlayerElimination = 1,
	Chest = 2,
	SupplyDrop = 3,
	AmmoBox = 4,
	Drone = 5,
	ItemSpawner = 6
};

enum class EFortPickupSourceTypeFlag : uint8
{
	Other = 0,
	Player = 1,
	Destruction = 2,
	Container = 3,
	AI = 4,
	Tossed = 5,
	FloorLoot = 6
};

class FFortPickupLocationData
{
	GENERATED_USTRUCT_BODY(FFortPickupLocationData, FortniteGame)

public:

	UPROPERTY_STRUCT(AFortPawn*, PickupTarget)
	AFortPawn* PickupTarget;

	UPROPERTY_STRUCT(AFortPawn*, ItemOwner)
	AFortPawn* ItemOwner;

	UPROPERTY_STRUCT(float, FlyTime)
	float FlyTime;

	UPROPERTY_STRUCT(FGuid, PickupGuid)
	FGuid PickupGuid;
};

class AFortPickup : public AActor
{
	GENERATED_UCLASS_BODY(AFortPickup, FortniteGame)
public:

	UPROPERTY_NOTIFY(FFortPickupLocationData, PickupLocationData)
	FFortPickupLocationData PickupLocationData;

	UPROPERTY_BOOL_NOTIFY(bPickedUp)
	bool bPickedUp;

	UPROPERTY(FFortItemEntry, PrimaryPickupItemEntry)
	FFortItemEntry ItemEntry;

	UPROPERTY(AFortPawn*, PawnWhoDroppedPickup)
	AFortPawn* PawnWhoDroppedPickup;

	UPROPERTY(UProjectileMovementComponent*, MovementComponent)
	UProjectileMovementComponent* MovementComponent;

	void OnRep_ItemEntry();

	void TossPickup(FVector FinalLocation, AFortPawn* ItemOwner, int OverrideMaxStackCount, bool bToss, EFortPickupSourceTypeFlag InPickupSourceTypeFlags, EFortPickupSpawnSource InPickupSpawnSource);

	void OnAboutToEnterBackpack();
};