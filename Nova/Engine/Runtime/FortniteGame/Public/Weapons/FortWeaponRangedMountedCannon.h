#pragma once
#include "FortWeaponRangedForVehicle.h"

class AFortWeaponRangedMountedCannon : public AFortWeaponRangedForVehicle
{
	GENERATED_UCLASS_BODY(AFortWeaponRangedMountedCannon, FortniteGame)
public:

	void ServerFireActorInCannon(FVector LaunchDir);
};