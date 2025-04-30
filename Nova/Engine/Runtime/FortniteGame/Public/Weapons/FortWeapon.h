#pragma once
#include "GameFramework/Actor.h"
#include "ItemDefinitions/FortWeaponItemDefinition.h"

class AFortWeapon : public AActor
{
	GENERATED_UCLASS_BODY(AFortWeapon, FortniteGame)
public:

	UPROPERTY(UFortWeaponItemDefinition*, WeaponData)
	UFortWeaponItemDefinition* WeaponData;

	UPROPERTY(FGuid, ItemEntryGuid)
	FGuid ItemEntryGuid;

	UPROPERTY(int, AmmoCount)
	int AmmoCount;

	UPROPERTY(bool, bIsPlayingFireFX)
	bool bIsPlayingFireFX;

	void Tick(float DeltaSeconds);
};