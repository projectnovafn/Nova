#include "Vehicles/FortAthenaVehicle.h"
#include "Vehicles/FortAthenaFerretVehicle.h"
#include "Vehicles/FortOctopusVehicle.h"
#include "Vehicles/FortAthenaSKPushCannon.h"
#include "Vehicles/FortMountedTurret.h"

#include "Player/FortPlayerController.h"

AFortPlayerPawn* AFortAthenaVehicle::GetDriver()
{
	AFortPlayerPawn* ReturnValue;
	this->ProcessEvent(L"GetDriver", &ReturnValue);

	return ReturnValue;
}

AFortPlayerPawn* AFortAthenaVehicle::GetPawnAtSeat(int SeatIdx)
{
	struct
	{
		int SeatIdx;
		AFortPlayerPawn* ReturnValue;
	} params(SeatIdx);

	this->ProcessEvent(L"GetPawnAtSeat", &params);

	return params.ReturnValue;
}

UFortItemDefinition* AFortAthenaVehicle::GetVehicleWeapon(int SeatIndex)
{
	UE_LOG(LogFort, Log, TEXT("GetVehicleWeapon: %s, %s"), *GetName(), *GetClass()->GetName());

	if (IsA<AFortAthenaFerretVehicle>() && SeatIndex == 0)
	{
		return FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Weapons/Ferret_Weapon.Ferret_Weapon");
	}

	if (IsA<AFortOctopusVehicle>())
	{
		return FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Weapons/Vehicles/WID_Octopus_Weapon.WID_Octopus_Weapon");
	}

	if (IsA<AFortAthenaSKPushCannon>())
	{
		return SeatIndex ? FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Weapons/Vehicles/ShipCannon_Weapon_InCannon.ShipCannon_Weapon_InCannon") :
			FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Weapons/Vehicles/ShipCannon_Weapon.ShipCannon_Weapon");
	}

	if (IsA<AFortMountedTurret>())
	{
		return FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Traps/MountedTurret/MountedTurret_Weapon.MountedTurret_Weapon");
	}

	UE_LOG(LogFort, Log, TEXT("GetVehicleWeapon return NULL"));

	return NULL;
}