#pragma once
#include "FortGameMode.h"
#include "ItemDefinitions/FortItem.h"

class FItemAndCount
{
	GENERATED_USTRUCT_BODY(FItemAndCount, FortniteGame)
public:

	UPROPERTY_STRUCT(int, Count)
	int Count;

	UPROPERTY_STRUCT(UFortItemDefinition*, Item)
	UFortItemDefinition* Item;
};

class AFortGameModeZone : public AFortGameMode
{
	GENERATED_UCLASS_BODY(AFortGameModeZone, FortniteGame)
public:

	UPROPERTY(TArray<FItemAndCount>, StartingItems)
	TArray<FItemAndCount> StartingItems;

	UPROPERTY_BOOL(bAllowSpectateAfterDeath)
	bool bAllowSpectateAfterDeath;
};