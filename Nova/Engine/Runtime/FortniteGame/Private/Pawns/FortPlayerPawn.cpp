#include "Pawns/FortPlayerPawn.h"
#include "Player/FortPlayerController.h"

#include "Weapons/FortWeaponRangedForVehicle.h"
#include "Weapons/FortWeaponRangedDualForVehicle.h"

void AFortPlayerPawn::ServerHandlePickup(AFortPickup* Pickup, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound)
{
	auto PlayerController = Cast<AFortPlayerController>(Controller);

	if (!PlayerController || !Pickup)
		return;

	float Distance = GetDistanceTo2D(Pickup) / 1000.0f;

	if (Distance > 10.0f)
	{
		return PlayerController->ClientReturnToMainMenu(L"You have been kicked due to an illegal move");
	}

	if (!Pickup->bPickedUp)
	{
		Pickup->PickupLocationData.PickupTarget = this;
		Pickup->PickupLocationData.ItemOwner = this;
		Pickup->PickupLocationData.FlyTime = 0.40;

		Pickup->OnRep_PickupLocationData();

		Pickup->bPickedUp = true;
	}
}

void AFortPlayerPawn::ServerHandlePickupWithSwap(AFortPickup* Pickup, FGuid Swap, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound)
{
	ServerHandlePickup(Pickup, InFlyTime, InStartDirection, bPlayPickupSound);
}

void AFortPlayerPawn::ServerHandlePickupWithRequestedSwap(AFortPickup* Pickup, FGuid Swap, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound)
{
	ServerHandlePickupWithSwap(Pickup, Swap, InFlyTime, InStartDirection, bPlayPickupSound);
}

AFortAthenaVehicle* AFortPlayerPawn::ServerOnExitVehicle(ETryExitVehicleBehavior ExitForceBehavior)
{
	struct
	{
		ETryExitVehicleBehavior ExitForceBehavior;
		AFortAthenaVehicle* ReturnValue;
	} params(ExitForceBehavior);

	this->ProcessEvent(L"ServerOnExitVehicle", &params);

	return params.ReturnValue;
}

void AFortPlayerPawn::OnRep_InVehicle()
{
	UE_LOG(LogFort, Log, TEXT("OnRep_IsInVehicle: %s"), IsInVehicle() ? L"true" : L"false");

	auto PlayerController = Cast<AFortPlayerController>(Controller);

	if (PlayerController == NULL)
		return;

	auto WorldInventory = PlayerController->WorldInventory;

	if (WorldInventory == NULL)
		return;

	if (IsInVehicle())
	{
		auto LastVehicle = Cast<AFortAthenaVehicle>(VehicleStateLastTick.Vehicle);
		auto LastVehicleWeapon = LastVehicle != NULL ? LastVehicle->GetVehicleWeapon(VehicleStateLastTick.SeatIndex) : NULL;

		if (LastVehicleWeapon != NULL)
		{
			auto ItemEntry = WorldInventory->FindItemEntry(LastVehicleWeapon);

			if (ItemEntry != NULL)
				WorldInventory->RemoveItem(ItemEntry->ItemGuid, ItemEntry->Count);
		}

		auto Vehicle = GetVehicle();
		auto VehicleWeapon = Vehicle->GetVehicleWeapon(VehicleStateLocal.SeatIndex);

		if (VehicleWeapon == NULL)
		{
			if (LastVehicleWeapon != NULL)
			{
				QueueTask(ENamedThreads::GameThread, 0.05,
					[PlayerController, WorldInventory] { PlayerController->ServerExecuteInventoryItem(WorldInventory->GetPickaxeItem()); });
			}

			return;
		}

		bool NeedsUpdate;
		bool WasSwapped;
		WorldInventory->AddItem(VehicleWeapon, 1, NeedsUpdate, INT32_MAX, WasSwapped, true);

		if (NeedsUpdate)
			WorldInventory->Update();

		PlayerController->ServerExecuteInventoryItem(WorldInventory->GetRecentItem());

		if (CurrentWeapon->GetClassField(L"MountedWeaponInfoRepped") != NULL)
		{
			if (auto WeaponRangedForVehicle = Cast<AFortWeaponRangedForVehicle>(CurrentWeapon))
			{
				WeaponRangedForVehicle->MountedWeaponInfoRepped.HostVehicleCachedActor = Vehicle;
				WeaponRangedForVehicle->MountedWeaponInfoRepped.HostVehicleSeatIndexCached = VehicleStateLocal.SeatIndex;
			}
			else if (auto WeaponRangedDualForVehicle = Cast<AFortWeaponRangedDualForVehicle>(CurrentWeapon))
			{
				WeaponRangedDualForVehicle->MountedWeaponInfoRepped.HostVehicleCachedActor = Vehicle;
				WeaponRangedDualForVehicle->MountedWeaponInfoRepped.HostVehicleSeatIndexCached = VehicleStateLocal.SeatIndex;
			}
		}
	}
	else
	{
		auto Vehicle = Cast<AFortAthenaVehicle>(VehicleStateLastTick.Vehicle);
		auto VehicleWeapon = Vehicle->GetVehicleWeapon(VehicleStateLastTick.SeatIndex);

		UE_LOG(LogFort, Log, TEXT("VehicleWeapon: %s"), *VehicleWeapon->GetName());

		if (VehicleWeapon == NULL)
			return;

		auto ItemEntry = WorldInventory->FindItemEntry(VehicleWeapon);

		UE_LOG(LogFort, Log, TEXT("ItemEntry: %p"), ItemEntry);

		if (ItemEntry == NULL)
			return;

		if (WorldInventory->RemoveItem(ItemEntry->ItemGuid, ItemEntry->Count))
			WorldInventory->Update();

		QueueTask(ENamedThreads::GameThread, 0.05,
			[PlayerController, WorldInventory] { PlayerController->ServerExecuteInventoryItem(WorldInventory->GetPickaxeItem()); });
	}
}

AFortAthenaVehicle* AFortPlayerPawn::GetVehicle()
{
	AFortAthenaVehicle* ReturnValue;
	this->ProcessEvent(L"GetVehicle", &ReturnValue);

	return ReturnValue;
}

bool AFortPlayerPawn::IsInVehicle()
{
	bool ReturnValue;
	this->ProcessEvent(L"IsInVehicle", &ReturnValue);

	return ReturnValue;
}