#include "Patches/FortKismetLibraryPatch.h"
#include "Kismet/KismetArrayLibrary.h"

#include "KismetDetoursLibrary.h"

static AFortPickup* (*_SpawnPickupInWorld)(UObject*, UFortWorldItemDefinition*, int, FVector, const FVector&, int, bool, bool, bool, int, EFortPickupSourceTypeFlag, EFortPickupSpawnSource);
static bool (*_PickLootDrops)(UObject*, TArray<FFortItemEntry>*, FName, int, int);
static void (*_GiveItemToInventoryOwner)(TScriptInterface<UObject>, UFortWorldItemDefinition*, int, bool, int, int);

AFortPickup* UFortKismetLibraryPatch::SpawnPickupInWorldHook(UObject* WorldContextObject, UFortWorldItemDefinition* ItemDefinition, int NumberToSpawn, FVector Position, const FVector& Direction, int OverrideMaxStackCount, bool bToss, bool bRandomRotation, bool bBlockedFromAutoPickup, int PickupInstigatorHandle, EFortPickupSourceTypeFlag SourceType, EFortPickupSpawnSource Source)
{
	auto World = WorldContextObject->GetWorld();

	if (auto Actor = Cast<AActor>(WorldContextObject))
		World = Actor->GetWorld();

	return UFortKismetLibrary::SpawnPickup(
		World, 
		Position, 
		ItemDefinition, 
		NumberToSpawn, 
		-1, 
		Cast<AFortPlayerController>(WorldContextObject),
		SourceType,
		Source);
}

bool UFortKismetLibraryPatch::PickLootDropsHook(UObject* WorldContextObject, TArray<FFortItemEntry>* OutLootToDrop, FName TierGroupName, int WorldLevel, int ForcedLootTier)
{
	static auto PickLootDropsFunc = FindObject<UFunction>(L"/Script/FortniteGame.FortKismetLibrary.PickLootDrops");

	auto World = WorldContextObject->GetWorld();

	if (auto Actor = Cast<AActor>(WorldContextObject))
		World = Actor->GetWorld();

	auto LootDrops = UFortKismetLibrary::PickLootDrops(World, TierGroupName, ForcedLootTier);

	for (auto LootDrop : LootDrops)
		UKismetArrayLibrary::Array_Add(*OutLootToDrop, PickLootDropsFunc->GetProperty(L"OutLootToDrop"), *LootDrop);

	return !LootDrops.empty();
}

void UFortKismetLibraryPatch::GiveItemToInventoryOwnerHook(TScriptInterface<UObject> InventoryOwner, UFortWorldItemDefinition* ItemDefinition, int NumberToGive, bool bNotifyPlayer, int ItemLevel, int PickupInstigatorHandle)
{
	UFortKismetLibrary::GiveItemToInventoryOwner(Cast<AFortPlayerController>(InventoryOwner.GetObject()), 
		ItemDefinition, NumberToGive, bNotifyPlayer, ItemLevel, PickupInstigatorHandle);
}

void UFortKismetLibraryPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<UFortKismetLibrary>(
		L"K2_SpawnPickupInWorld", 
		EReflectedDetourType::ImplSwap, 
		SpawnPickupInWorldHook, 
		_SpawnPickupInWorld
	);

	UKismetDetoursLibrary::AddReflectedDetour<UFortKismetLibrary>(
		L"PickLootDrops",
		EReflectedDetourType::ImplSwap,
		PickLootDropsHook,
		_PickLootDrops,
		0,
		{ 0x89, 0x44 }
	);

	UKismetDetoursLibrary::AddReflectedDetour<UFortKismetLibrary>(
		L"GiveItemToInventoryOwner",
		EReflectedDetourType::ImplSwap,
		GiveItemToInventoryOwnerHook,
		_GiveItemToInventoryOwner,
		0,
		{ 0x89, 0x44 }
	);
}