#pragma once
#include "FortWeapon.h"

class FMountedWeaponInfoRepped
{
	GENERATED_USTRUCT_BODY(FMountedWeaponInfoRepped, FortniteGame)
public:

	UPROPERTY_STRUCT(AActor*, HostVehicleCachedActor)
	AActor* HostVehicleCachedActor;

	UPROPERTY_STRUCT(int, HostVehicleSeatIndexCached)
	int HostVehicleSeatIndexCached;
};

class AFortWeaponRanged : public AFortWeapon
{
	GENERATED_UCLASS_BODY(AFortWeaponRanged, FortniteGame)
public:


};