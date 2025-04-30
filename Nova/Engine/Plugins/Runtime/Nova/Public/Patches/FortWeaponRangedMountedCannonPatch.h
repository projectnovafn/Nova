#pragma once
#include "Weapons/FortWeaponRangedMountedCannon.h"

class AFortWeaponRangedMountedCannonPatch
{
private:
	static void ServerFireActorInCannonHook(AFortWeaponRangedMountedCannon*, FVector);

public:

	static void Init();
};