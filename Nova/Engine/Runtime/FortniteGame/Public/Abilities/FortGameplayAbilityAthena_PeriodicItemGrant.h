#pragma once
#include "FortGameplayAbility.h"

struct FActiveItemGrantInfo
{
	UFortWorldItemDefinition* Item;
	FScalableFloat AmountToGive;
	FScalableFloat MaxAmount;

	bool operator==(const FActiveItemGrantInfo& Other) const
	{
		return Item == Other.Item;
	}
};

FORCEINLINE uint32 GetTypeHash(const FActiveItemGrantInfo& Info)
{
	return GetTypeHash(Info.Item);
}

class UFortGameplayAbilityAthena_PeriodicItemGrant : public UFortGameplayAbility
{
	GENERATED_UCLASS_BODY(UFortGameplayAbilityAthena_PeriodicItemGrant, FortniteGame)
public:

	typedef TMap<FActiveItemGrantInfo, FScalableFloat> ItemsToGrantType;

	UPROPERTY(ItemsToGrantType, ItemsToGrant)
	ItemsToGrantType ItemsToGrant;

	UPROPERTY(TArray<FTimerHandle>, ActiveTimers)
	TArray<FTimerHandle> ActiveTimers;

	UPROPERTY(AFortPlayerController*, OwnerControllerCachedValue)
	AFortPlayerController* OwnerControllerCachedValue;

	void StopItemAwardTimers();
	void StartItemAwardTimers();
};