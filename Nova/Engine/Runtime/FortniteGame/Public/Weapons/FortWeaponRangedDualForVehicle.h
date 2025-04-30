#pragma once
#include "FortWeaponRangedDual.h"

class AFortWeaponRangedDualForVehicle : public AFortWeaponRangedDual
{
	GENERATED_UCLASS_BODY(AFortWeaponRangedDualForVehicle, FortniteGame)
public:

	UPROPERTY(FMountedWeaponInfoRepped, MountedWeaponInfoRepped)
	FMountedWeaponInfoRepped MountedWeaponInfoRepped;
};