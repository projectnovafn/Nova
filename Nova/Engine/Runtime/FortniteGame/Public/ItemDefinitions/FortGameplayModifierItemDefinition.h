#pragma once
#include "FortItem.h"

class UFortAbilitySet;

class FFortAbilitySetDeliveryInfo
{
	GENERATED_USTRUCT_BODY(FFortAbilitySetDeliveryInfo, FortniteGame)
public:

	UPROPERTY_STRUCT(TArray<TSoftObjectPtr<UFortAbilitySet>>, AbilitySets)
	TArray<TSoftObjectPtr<UFortAbilitySet>> AbilitySets;
};

class UFortGameplayModifierItemDefinition : public UFortItemDefinition
{
	GENERATED_UCLASS_BODY(UFortGameplayModifierItemDefinition, FortniteGame)
public:

	UPROPERTY(TArray<FFortAbilitySetDeliveryInfo>, PersistentAbilitySets)
	TArray<FFortAbilitySetDeliveryInfo> PersistentAbilitySets;
};