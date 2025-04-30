#include "Building/FortAthenaSupplyDrop.h"

#include "Kismet/FortKismetLibrary.h"

void AFortAthenaSupplyDrop::SpawnLoot(AFortPlayerPawn* Pawn)
{
	auto Instigator = Pawn ? Cast<AFortPlayerController>(Pawn->GetOwner()) : NULL;

	auto LootTier = LootTableNameOverride != NAME_None ? LootTableNameOverride : L"Loot_AthenaSupplyDrop";
	auto LootDrops = UFortKismetLibrary::PickLootDrops(GetWorld(), LootTier);

	for (auto& LootDrop : LootDrops)
		UFortKismetLibrary::SpawnPickup(GetWorld(), GetActorLocation(), LootDrop->ItemDefinition, LootDrop->Count, LootDrop->LoadedAmmo, Instigator, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::SupplyDrop);

	this->DestroyActor();
}