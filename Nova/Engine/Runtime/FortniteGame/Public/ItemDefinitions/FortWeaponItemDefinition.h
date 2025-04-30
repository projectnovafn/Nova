#pragma once

#include "Inventory/FortWorldItem.h"
#include "Engine/DataTable.h"

class UFortWeaponItemDefinition : public UFortWorldItemDefinition
{
	GENERATED_UCLASS_BODY(UFortWeaponItemDefinition, FortniteGame)

public:

	UPROPERTY(FDataTableRowHandle, WeaponStatHandle)
	FDataTableRowHandle WeaponStatHandle;
};