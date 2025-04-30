#pragma once
#include "FortPawn.h"
#include "Inventory/FortPickup.h"
#include "Vehicles/FortAthenaVehicle.h"

enum class ETryExitVehicleBehavior : uint8
{
	DoNotForce = 0,
	ForceOnBlocking = 1,
	ForceAlways = 2
};

class AFortPlayerPawn : public AFortPawn
{
	GENERATED_UCLASS_BODY(AFortPlayerPawn, FortniteGame)
public:

	UPROPERTY(UClass*, TeammateReviveGameplayEffect)
	UClass* TeammateReviveGameplayEffect;

	UPROPERTY(FVehiclePawnState, VehicleStateLocal)
	FVehiclePawnState VehicleStateLocal;

	UPROPERTY(FVehiclePawnState, VehicleStateLastTick)
	FVehiclePawnState VehicleStateLastTick;

	UPROPERTY(bool, bIsParachuteOpen)
	bool bIsParachuteOpen;

	__declspec(property(get = InternalGetLastEquippedWeaponOrGadgetGUID))
	FGuid LastEquippedWeaponOrGadgetGUID;

	FGuid InternalGetLastEquippedWeaponOrGadgetGUID();

	AFortAthenaVehicle* GetVehicle();
	bool IsInVehicle();
	void OnRep_InVehicle();

	void ServerHandlePickup(AFortPickup* Pickup, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound);
	void ServerHandlePickupWithSwap(AFortPickup* Pickup, FGuid Swap, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound);
	void ServerHandlePickupWithRequestedSwap(AFortPickup* Pickup, FGuid Swap, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound);

	AFortAthenaVehicle* ServerOnExitVehicle(ETryExitVehicleBehavior ExitForceBehavior);
};