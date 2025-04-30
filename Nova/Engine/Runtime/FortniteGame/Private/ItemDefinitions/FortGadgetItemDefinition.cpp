#include "ItemDefinitions/FortGadgetItemDefinition.h"

UFortWeaponItemDefinition* UFortGadgetItemDefinition::GetWeaponItemDefinition()
{
	UFortWeaponItemDefinition* ReturnValue;
	this->ProcessEvent(L"GetWeaponItemDefinition", &ReturnValue);

	return ReturnValue;
}

bool UFortGadgetItemDefinition::ApplyGadgetData(void* InventoryInterface, UFortItem* ItemInstance)
{
	static auto ApplyGadgetData = UKismetMemoryLibrary::Get<bool (*)(UFortGadgetItemDefinition*, void*, UFortItem*, bool)>(L"UFortGadgetItemDefinition::ApplyGadgetData");

	return ApplyGadgetData(this, InventoryInterface, ItemInstance, true);
}