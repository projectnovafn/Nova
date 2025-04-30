#include "Building/BuildingItemCollectorActor.h"

#include "Kismet/FortKismetLibrary.h"

#include "ItemDefinitions/FortResourceItemDefinition.h"

UFortWorldItemDefinition* ABuildingItemCollectorActor::GetCurrentActiveItem()
{
	UFortWorldItemDefinition* ReturnValue;
	this->ProcessEvent(L"GetCurrentActiveItem", &ReturnValue);

	return ReturnValue;
}

void ABuildingItemCollectorActor::PickupSpawned_Bind()
{
	this->ProcessEvent(L"PickupSpawned_Bind");
}

void ABuildingItemCollectorActor::SpawnLoot(AFortPlayerPawn* Pawn)
{
	static auto StoneItemData = FindObject<UFortResourceItemDefinition>(L"/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
	static auto MetalItemData = FindObject<UFortResourceItemDefinition>(L"/Game/Items/ResourcePickups/MetalItemData.MetalItemData");

	int Index = 0;
	if (ActiveInputItem == StoneItemData) Index = 1;
	else if (ActiveInputItem == MetalItemData) Index = 2;

	auto CorrectLocation = GetActorLocation() + GetActorRightVector() * 70.0f + FVector(0, 0, 50);
	auto& ItemCollection = ItemCollections[Index];

	for (auto& OutputItem : ItemCollection.OutputItemEntry)
	{
		if (auto Pickup = UFortKismetLibrary::SpawnPickup(GetWorld(), CorrectLocation, OutputItem.ItemDefinition, OutputItem.Count))
		{
			if (auto WeaponItemDef = Cast<UFortWeaponItemDefinition>(OutputItem.ItemDefinition))
			{
				Pickup->ItemEntry.LoadedAmmo = UFortKismetLibrary::GetClipSize(WeaponItemDef);
			}
		}
	}

	this->bCurrentInteractionSuccess = true;
	this->PickupSpawned_Bind();
}