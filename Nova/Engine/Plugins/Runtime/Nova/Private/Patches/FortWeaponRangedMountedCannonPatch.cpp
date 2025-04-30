#include "Patches/FortWeaponRangedMountedCannonPatch.h"

#include "KismetDetoursLibrary.h"

static void (*_ServerFireActorInCannon)(AFortWeaponRangedMountedCannon*, FVector);

void AFortWeaponRangedMountedCannonPatch::ServerFireActorInCannonHook(AFortWeaponRangedMountedCannon* Cannon, FVector LaunchDir)
{
	Cannon->ServerFireActorInCannon(LaunchDir);
}

void AFortWeaponRangedMountedCannonPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<AFortWeaponRangedMountedCannon>(
		L"ServerFireActorInCannon",
		EReflectedDetourType::VFSwap,
		ServerFireActorInCannonHook,
		_ServerFireActorInCannon,
		1
	);
}