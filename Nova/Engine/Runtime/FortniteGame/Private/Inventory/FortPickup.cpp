#include "Inventory/FortPickup.h"
#include "Pawns/FortPlayerPawn.h"
#include "Player/FortPlayerController.h"

#include "Kismet/FortKismetLibrary.h"

void AFortPickup::OnRep_ItemEntry()
{
	this->ProcessEvent(L"OnRep_PrimaryPickupItemEntry");
}

void AFortPickup::TossPickup(FVector FinalLocation, AFortPawn* ItemOwner, int OverrideMaxStackCount, bool bToss, EFortPickupSourceTypeFlag InPickupSourceTypeFlags, EFortPickupSpawnSource InPickupSpawnSource)
{
	struct
	{
		FVector FinalLocation;
		AFortPawn* ItemOwner;
		int OverrideMaxStackCount;
		bool bToss;
		EFortPickupSourceTypeFlag InPickupSourceTypeFlags;
		EFortPickupSpawnSource InPickupSpawnSource;
	} params(FinalLocation, ItemOwner, OverrideMaxStackCount, bToss, InPickupSourceTypeFlags, InPickupSpawnSource);

	this->ProcessEvent(L"TossPickup", &params);
}

void AFortPickup::OnAboutToEnterBackpack()
{
	auto Pawn = Cast<AFortPlayerPawn>(PickupLocationData.ItemOwner);

	if (!Pawn)
		return;

	auto PlayerController = Cast<AFortPlayerController>(Pawn->Controller);

	if (!PlayerController)
		return;

	auto WorldInventory = PlayerController->WorldInventory;

	if (!WorldInventory)
		return;

	bool NeedsUpdate;
	bool WasSwapped;

	WorldInventory->AddItem(ItemEntry.ItemDefinition, ItemEntry.Count, NeedsUpdate, ItemEntry.LoadedAmmo, WasSwapped);
	WorldInventory->Update();

	if (WasSwapped)
		PlayerController->ClientEquipItem(WorldInventory->GetRecentItem(), false);
}