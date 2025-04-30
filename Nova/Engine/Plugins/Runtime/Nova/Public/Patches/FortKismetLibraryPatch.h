#pragma once
#include "Kismet/FortKismetLibrary.h"

class UFortKismetLibraryPatch
{
public:
	static AFortPickup* SpawnPickupInWorldHook(UObject*, UFortWorldItemDefinition*, int, FVector, const struct FVector&, int, bool bToss, bool, bool, int, EFortPickupSourceTypeFlag, EFortPickupSpawnSource);
	static bool PickLootDropsHook(UObject*, TArray<FFortItemEntry>*, FName, int, int);
	static void GiveItemToInventoryOwnerHook(TScriptInterface<UObject>, UFortWorldItemDefinition*, int, bool, int, int);

	static void Init();
};