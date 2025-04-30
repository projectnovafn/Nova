#include "Inventory/FortInventory.h"
#include "Inventory/FortWorldItem.h"
#include "Player/FortPlayerController.h"
#include "Kismet/KismetArrayLibrary.h"

#include "ItemDefinitions/FortWeaponMeleeItemDefinition.h"
#include "ItemDefinitions/FortGadgetItemDefinition.h"
#include "ItemDefinitions/FortBuildingItemDefinition.h"
#include "ItemDefinitions/FortResourceItemDefinition.h"
#include "ItemDEfinitions/FortAmmoItemDefinition.h"
#include "ItemDefinitions/FortTrapItemDefinition.h"

#include "Kismet/FortKismetLibrary.h"

bool AFortInventory::IsPrimaryQuickbar(UFortItemDefinition* ItemDefinition)
{
	return (!ItemDefinition->IsA<UFortWeaponMeleeItemDefinition>() &&
		!ItemDefinition->IsA<UFortBuildingItemDefinition>() &&
		!ItemDefinition->IsA<UFortAmmoItemDefinition>() &&
		!ItemDefinition->IsA<UFortResourceItemDefinition>() &&
		!ItemDefinition->IsA<UFortTrapItemDefinition>());
}

bool AFortInventory::IsInventoryFull(int Start)
{
	int ItemCounts = Start;

	for (auto& ItemInstance : Inventory.ItemInstances)
	{
		if (!ItemInstance)
			continue;

		auto ItemDefinition = ItemInstance->ItemEntry.ItemDefinition;

		if (IsPrimaryQuickbar(ItemDefinition))
			ItemCounts++;
	}

	return ItemCounts >= 7;
}

std::vector<UFortWorldItem*> AFortInventory::FindAllInstances(UFortItemDefinition* ItemDef, std::vector<std::function<bool(UFortWorldItem*)>> Conditions)
{
	std::vector<UFortWorldItem*> Instances{};

	for (auto& ItemInstance : Inventory.ItemInstances)
	{
		bool bPassedConditions = true;

		for (auto Condition : Conditions)
		{
			if (!Condition(ItemInstance))
				bPassedConditions = false;
		}

		if (!bPassedConditions)
			continue;

		if (ItemInstance && (ItemInstance->ItemEntry.ItemDefinition == ItemDef || ItemDef == NULL))
			Instances.emplace_back(ItemInstance);
	}

	return Instances;
}

std::vector<UFortWorldItem*> AFortInventory::GetStackableItems(UFortItemDefinition* ItemDef, int Count)
{
	return FindAllInstances(ItemDef,
	{
		[Count, ItemDef](UFortWorldItem* ItemInstance) -> bool
		{
			if (!ItemInstance)
				return false;

			if (ItemDef->MaxStackSize == ItemInstance->ItemEntry.Count)
				return false;

			return true;
		}
	});
}

std::vector<UFortWorldItem*> AFortInventory::GetDroppableItems(UFortItemDefinition* IgnoreDef)
{
	return FindAllInstances(NULL,
	{
		[IgnoreDef](UFortWorldItem* ItemInstance) -> bool
		{
			if (!ItemInstance)
				return false;

			if (ItemInstance->ItemEntry.ItemDefinition == IgnoreDef)
				return false;

			if (!ItemInstance->CanBeDropped())
				return false;

			return true;
		}
	});
}

FFortItemEntry* AFortInventory::FindItemEntry(FGuid ItemGuid)
{
	for (auto& ItemEntry : Inventory.ReplicatedEntries)
	{
		if (ItemEntry.ItemGuid == ItemGuid)
			return &ItemEntry;
	}

	return NULL;
}

UFortWorldItem* AFortInventory::FindItemInstance(FGuid ItemGuid)
{
	for (auto& ItemInstance : Inventory.ItemInstances)
	{
		if (ItemInstance->ItemEntry.ItemGuid == ItemGuid)
			return ItemInstance;
	}

	return NULL;
}

FFortItemEntry* AFortInventory::FindItemEntry(UFortItemDefinition* ItemDefinition)
{
	for (auto& ItemEntry : Inventory.ReplicatedEntries)
	{
		if (ItemEntry.ItemDefinition == ItemDefinition)
			return &ItemEntry;
	}

	return NULL;
}

UFortWorldItem* AFortInventory::FindItemInstance(UFortItemDefinition* ItemDefinition)
{
	for (auto& ItemInstance : Inventory.ItemInstances)
	{
		if (ItemInstance->ItemEntry.ItemDefinition == ItemDefinition)
			return ItemInstance;
	}

	return NULL;
}

bool AFortInventory::RemoveItem(FGuid ItemGuid, int Count)
{
	auto ItemEntry = FindItemEntry(ItemGuid);
	auto ItemInstance = FindItemInstance(ItemGuid);

	if (!ItemEntry || !ItemInstance)
	{
		return false;
	}

	auto NewCount = ItemEntry->Count - Count;

	if (NewCount <= 0)
	{
		UKismetArrayLibrary::Array_RemoveItem(Inventory.ItemInstances, FFortItemList::StaticStruct()->GetProperty(L"ItemInstances"), ItemInstance);
		UKismetArrayLibrary::Array_RemoveItem(Inventory.ReplicatedEntries, FFortItemList::StaticStruct()->GetProperty(L"ReplicatedEntries"), *ItemEntry);

		if (auto PlayerController = GetOwningPlayer())
		{
			if (auto Pawn = PlayerController->MyFortPawn)
			{
				for (int i = 0; i < Pawn->CurrentWeaponList.Num(); i++)
				{
					auto Weapon = Pawn->CurrentWeaponList[i];

					if (Weapon && Weapon->ItemEntryGuid == ItemGuid)
					{
						Weapon->ItemEntryGuid = FGuid();

						UKismetArrayLibrary::Array_Remove(Pawn->CurrentWeaponList, Pawn->GetClassProperty(L"CurrentWeaponList"), i);
						break;
					}
				}
			}
		}

		return true;
	}
	else
	{
		ItemInstance->ItemEntry.Count = NewCount;
		ItemEntry->Count = NewCount;

		Inventory.MarkItemDirty(*ItemEntry);
	}

	return false;
}

void AFortInventory::Clear()
{
	for (auto ItemInstance : GetDroppableItems())
	{
		this->RemoveItem(ItemInstance->ItemEntry.ItemGuid, ItemInstance->ItemEntry.Count);
	}

	this->Update();
}

void AFortInventory::UpdateItem(FGuid ItemGuid, int Count, int LoadedAmmo, AFortWeapon* Weapon)
{
	auto ItemInstance = this->FindItemInstance(ItemGuid);
	auto ItemEntry = this->FindItemEntry(ItemGuid);

	if (!ItemEntry || !ItemInstance)
		return;

	if ((Count == -1 || ItemEntry->Count == Count) && (LoadedAmmo == -1 || ItemEntry->LoadedAmmo == LoadedAmmo))
		return;

	if (Count != -1)
	{
		ItemInstance->ItemEntry.Count = Count;
		ItemEntry->Count = Count;
	}

	if (LoadedAmmo != -1)
	{
		ItemInstance->ItemEntry.LoadedAmmo = LoadedAmmo;
		ItemEntry->LoadedAmmo = LoadedAmmo;
	}

	if (ItemEntry->LoadedAmmo == 0 && ItemEntry->ItemDefinition->GameplayTags.Contains(L"Item.Consumable.HookGun"))
	{
		if (auto PlayerController = GetOwningPlayer())
		{
			if (auto Pawn = PlayerController->MyFortPawn)
			{
				if (Pawn->bIsParachuteOpen)
					return;
			}
		}

		if (Weapon && Weapon->bIsPlayingFireFX)
			return;

		this->RemoveItem(ItemEntry->ItemGuid, ItemEntry->Count);
		this->Update();

		return;
	}

	Inventory.MarkItemDirty(*ItemEntry);
}

bool AFortInventory::AddItem(UFortItemDefinition* ItemDef, int Count, bool& NeedsUpdate, int LoadedAmmo, bool& WasSwapped, bool Force)
{
	NeedsUpdate = false;
	WasSwapped = false;

	if (Count >= ItemDef->MaxStackSize)
		Count = ItemDef->MaxStackSize;

	auto ItemInstances = GetStackableItems(ItemDef, Count);

	for (auto ItemInstance : ItemInstances)
	{
		int Added = FMath::Min(Count, ItemDef->MaxStackSize - ItemInstance->ItemEntry.Count);
		Count -= Added;
		ItemInstance->ItemEntry.Count += Added;

		auto ItemEntry = FindItemEntry(ItemInstance->ItemEntry.ItemGuid);
		ItemEntry->Count = ItemInstance->ItemEntry.Count;

		Inventory.MarkItemDirty(*ItemEntry);
	}

	if (ItemInstances.size() != 0 && Count == 0 && !Force)
		return true;

	auto PlayerController = GetOwningPlayer();
	auto Pawn = PlayerController ? PlayerController->MyFortPawn : NULL;

	if (FindItemEntry(ItemDef) != NULL && !ItemDef->bAllowMultipleStacks && !Force)
	{
		UFortKismetLibrary::SpawnPickup(GetWorld(), Pawn->GetActorLocation(), ItemDef, Count, 0, PlayerController);

		return true;
	}

	if (Pawn && IsPrimaryQuickbar(ItemDef) && IsInventoryFull(1) && Pawn->CurrentWeapon && !Force)
	{
		auto& CurrentItemGuid = Pawn->CurrentWeapon->ItemEntryGuid;

		if (auto ItemInstance = FindItemInstance(CurrentItemGuid))
		{
			auto ItemEntry = &ItemInstance->ItemEntry;

			if (!ItemEntry->ItemDefinition)
				return true;

			if (!ItemInstance->CanBeDropped())
			{
				UFortKismetLibrary::SpawnPickup(GetWorld(), Pawn->GetActorLocation(), ItemDef, Count, LoadedAmmo, PlayerController);

				return true;
			}

			UFortKismetLibrary::SpawnPickup(GetWorld(), Pawn->GetActorLocation(), ItemEntry->ItemDefinition, ItemEntry->Count, ItemEntry->LoadedAmmo, PlayerController);

			RemoveItem(CurrentItemGuid, ItemEntry->Count);

			NeedsUpdate = true;
			WasSwapped = true;
		}
		else
		{
			UFortKismetLibrary::SpawnPickup(GetWorld(), Pawn->GetActorLocation(), ItemDef, Count, LoadedAmmo, PlayerController);

			return true;
		}
	}

	if (auto ItemInstance = ItemDef->Create<UFortWorldItem>(Count))
	{
		if (auto OwningPlayer = GetOwningPlayer())
		{
			ItemInstance->SetOwner(OwningPlayer);
		}

		if (auto GadgetItemDef = Cast<UFortGadgetItemDefinition>(ItemDef))
		{
			GadgetItemDef->ApplyGadgetData(PlayerController->GetInterfaceAddress(UFortInventoryOwnerInterface::StaticClass()), ItemInstance);
		}

		auto StateValue = FFortItemEntryStateValue::Allocate();
		StateValue->IntValue = 1;
		StateValue->StateType = EFortItemEntryState::ShouldShowItemToast;

		UKismetArrayLibrary::Array_Add(ItemInstance->ItemEntry.StateValues, FFortItemEntry::StaticStruct()->GetProperty(L"StateValues"), *StateValue);

		FMemory::Free(StateValue);

		ItemInstance->ItemEntry.Count = Count;
		ItemInstance->ItemEntry.LoadedAmmo = LoadedAmmo;

		UKismetArrayLibrary::Array_Add(Inventory.ItemInstances, FFortItemList::StaticStruct()->GetProperty(L"ItemInstances"), ItemInstance);
		UKismetArrayLibrary::Array_Add(Inventory.ReplicatedEntries, FFortItemList::StaticStruct()->GetProperty(L"ReplicatedEntries"), ItemInstance->ItemEntry);

		NeedsUpdate = true;

		return true;
	}

	return false;
}

void AFortInventory::HandleInventoryLocalUpdate()
{
	this->ProcessEvent(L"HandleInventoryLocalUpdate");
}

void AFortInventory::Update()
{
	HandleInventoryLocalUpdate();

	if (auto OwningPlayer = GetOwningPlayer())
	{
		OwningPlayer->HandleWorldInventoryLocalUpdate();
		OwningPlayer->ClientForceUpdateQuickbar(EFortQuickBars::Primary);
		OwningPlayer->ClientForceUpdateQuickbar(EFortQuickBars::Secondary);
	}

	Inventory.MarkArrayDirty();
}

AFortPlayerController* AFortInventory::GetOwningPlayer()
{
	if (auto Player = Cast<AFortPlayerController>(GetOwner()))
	{
		return Player;
	}

	return NULL;
}

FGuid AFortInventory::GetRecentItem()
{
	return Inventory.ReplicatedEntries.Last().ItemGuid;
}

FGuid AFortInventory::GetPickaxeItem()
{
	for (auto& ItemEntry : Inventory.ReplicatedEntries)
	{
		if (ItemEntry.ItemDefinition && ItemEntry.ItemDefinition->IsA<UFortWeaponMeleeItemDefinition>())
		{
			return ItemEntry.ItemGuid;
		}
	}

	return FGuid();
}