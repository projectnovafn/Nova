#pragma once

#include "FortWeaponItemDefinition.h"

class UFortGadgetItemDefinition : public UFortWorldItemDefinition
{
	GENERATED_UCLASS_BODY(UFortGadgetItemDefinition, FortniteGame)

public:

	UFortWeaponItemDefinition* GetWeaponItemDefinition();
	bool ApplyGadgetData(void* InventoryInterface, UFortItem* ItemInstance);
};