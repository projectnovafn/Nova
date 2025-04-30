#include "Abilities/FortGameplayAbility.h"

void UFortGameplayAbility::EquipAbilityWeapon(UFortWeaponItemDefinition* WeaponDefinition, int ItemLevel)
{
	UE_LOG(LogFort, Log, TEXT("EquipAbilityWeapon: %p, %i"), WeaponDefinition, ItemLevel);
}