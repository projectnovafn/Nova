#pragma once
#include "CoreUObject.h"
#include "Engine/DataAsset.h"
#include "ItemDefinitions/FortItem.h"

#include "GameplayTagContainer.h"

class UFortItem;

class UFortItemDefinition : public UPrimaryDataAsset
{
	GENERATED_UCLASS_BODY(UFortItemDefinition, FortniteGame)
public:

	UPROPERTY_BOOL(bAllowMultipleStacks)
	bool bAllowMultipleStacks;

	UPROPERTY(int, MaxStackSize)
	int MaxStackSizeInt;

	UPROPERTY(uint8, Rarity)
	uint8 Rarity;

	UPROPERTY(FGameplayTagContainer, GameplayTags)
	FGameplayTagContainer GameplayTags;

	int GetMaxStackSize();

	__declspec(property(get = GetMaxStackSize))
	int MaxStackSize;

	UFortItem* Create(int32 Count, int32 Level = 1);

	template <typename T>
	T* Create(int32 Count = 1, int32 Level = 1)
	{
		return Cast<T>(Create(Count, Level));
	}
};

class UFortItem : public UObject
{
	GENERATED_UCLASS_BODY(UFortItem, FortniteGame)
public:

	FGuid GetItemGuid();
	bool CanBeDropped();
};