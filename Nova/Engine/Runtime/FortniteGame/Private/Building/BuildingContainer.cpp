#include "Building/BuildingContainer.h"
#include "Building/BuildingCustomizableSpawnContainer.h"

#include "Athena/FortGameModeAthena.h"
#include "Kismet/FortKismetLibrary.h"

void ABuildingContainer::SpawnLoot(AFortPlayerPawn* Pawn)
{
	auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->AuthorityGameMode);
	auto Instigator = Pawn ? Cast<AFortPlayerController>(Pawn->GetOwner()) : NULL;

	if (GameMode == NULL)
		return;

	if (auto CustomizableContainer = Cast<ABuildingCustomizableSpawnContainer>(this))
	{
		if (CustomizableContainer->SpawnItems.Num())
			return;
	}

	auto CorrectLocation = Pawn ? GetActorLocation() + GetActorRightVector() * 70.0f + FVector{ 0, 0, 50 } : GetActorLocation();
	auto LootTier = GameMode->RedirectLootTierGroup(SearchLootTierGroup);
	
	auto LootDrops = UFortKismetLibrary::PickLootDrops(GetWorld(), LootTier);

	for (auto& LootDrop : LootDrops)
	{
		UFortKismetLibrary::SpawnPickup(GetWorld(), CorrectLocation, LootDrop->ItemDefinition, LootDrop->Count, LootDrop->LoadedAmmo, Instigator);
	}
}

void ABuildingContainer::BounceContainer()
{
	SearchBounceData.SearchAnimationCount++;
}