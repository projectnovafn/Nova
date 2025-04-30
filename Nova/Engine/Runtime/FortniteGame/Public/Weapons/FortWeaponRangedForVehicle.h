#pragma once
#include "FortWeaponRanged.h"

class AFortWeaponRangedForVehicle : public AFortWeaponRanged
{
	GENERATED_UCLASS_BODY(AFortWeaponRangedForVehicle, FortniteGame)
public:

	UPROPERTY(FMountedWeaponInfoRepped, MountedWeaponInfoRepped)
	FMountedWeaponInfoRepped MountedWeaponInfoRepped;
};