#pragma once
#include "GameplayAbility.h"
#include "ItemDefinitions/FortWeaponItemDefinition.h"

class UFortGameplayAbility : public UGameplayAbility
{
	GENERATED_UCLASS_BODY(UFortGameplayAbility, FortniteGame)
public:

	void EquipAbilityWeapon(UFortWeaponItemDefinition* WeaponDefinition, int ItemLevel);
};