#pragma once

#include "Pawns/FortPhysicsPawn.h"
#include "ItemDefinitions/FortItem.h"

#include "UObject/WeakObjectPtr.h"

#include "Components/FortVehicleSeatWeaponComponent.h"

class AFortPlayerPawn;
class AFortPlayerController;
class UPrimitiveComponent;

enum class ETInteractionType : uint8
{
	IT_NoInteraction = 0,
	IT_Simple = 1,
	IT_LongPress = 2,
	IT_BuildingEdit = 3,
	IT_BuildingImprovement = 4,
	IT_TrapPlacement = 5,
	IT_MAX = 6
};

class FInteractionType
{
	GENERATED_USTRUCT_BODY(FInteractionType, FortniteGame)

public:

	UPROPERTY_STRUCT(ETInteractionType, InteractionType)
	ETInteractionType InteractionType;

	UPROPERTY_STRUCT(TWeakObjectPtr<AFortPlayerPawn>, RequestingPawn)
	TWeakObjectPtr<AFortPlayerPawn> RequestingPawn;

	UPROPERTY_STRUCT(TWeakObjectPtr<AFortPlayerController>, RequestingPlayerController)
	TWeakObjectPtr<AFortPlayerController> RequestingPlayerController;

	UPROPERTY_STRUCT(TWeakObjectPtr<UPrimitiveComponent>, InteractComponent)
	TWeakObjectPtr<UPrimitiveComponent> InteractComponent;

	UPROPERTY_STRUCT(TWeakObjectPtr<UObject>, OptionalObjectData)
	TWeakObjectPtr<UObject> OptionalObjectData;

	UPROPERTY_STRUCT(FVector, InteractPoint)
	FVector InteractPoint;
};

class FVehiclePawnState
{
	GENERATED_USTRUCT_BODY(FVehiclePawnState, FortniteGame)
public:

	UPROPERTY_STRUCT(AActor*, Vehicle)
	AActor* Vehicle;

	UPROPERTY_STRUCT(uint8, SeatIndex)
	uint8 SeatIndex;
};

class AFortAthenaVehicle : public AFortPhysicsPawn
{
	GENERATED_UCLASS_BODY(AFortAthenaVehicle, FortniteGame)

public:

	AFortPlayerPawn* GetDriver();
	UFortItemDefinition* GetVehicleWeapon(int SeatIndex);
	AFortPlayerPawn* GetPawnAtSeat(int SeatIdx);
};