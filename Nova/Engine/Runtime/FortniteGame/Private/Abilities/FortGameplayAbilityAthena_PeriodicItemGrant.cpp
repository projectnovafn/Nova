#include "Abilities/FortGameplayAbilityAthena_PeriodicItemGrant.h"

void UFortGameplayAbilityAthena_PeriodicItemGrant::StopItemAwardTimers()
{
	UE_LOG(LogFort, Log, TEXT("UFortGameplayAbilityAthena_PeriodicItemGrant::StopItemAwardTimers, %i"), ActiveTimers.Num());

	for (auto&& ItemToGrant : ItemsToGrant)
	{
		UE_LOG(LogFort, Log, TEXT("ItemToGrant, %s, %i"), *ItemToGrant.Key.Item->GetName(), ItemToGrant.Key.AmountToGive.Value);
	}
}

void UFortGameplayAbilityAthena_PeriodicItemGrant::StartItemAwardTimers()
{
	UE_LOG(LogFort, Log, TEXT("UFortGameplayAbilityAthena_PeriodicItemGrant::StartItemAwardTimers, %i"), ActiveTimers.Num());

	for (auto&& ItemToGrant : ItemsToGrant)
	{
		UE_LOG(LogFort, Log, TEXT("ItemToGrant, %s, %i"), *ItemToGrant.Key.Item->GetName(), ItemToGrant.Key.AmountToGive.Value);
	}
}