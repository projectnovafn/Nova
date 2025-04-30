#pragma once
#include "BuildingContainer.h"

class FFortItemQuantityPair
{
	GENERATED_USTRUCT_BODY(FFortItemQuantityPair, FortniteGame)
public:

	UPROPERTY_STRUCT(int, Quantity)
	int Quantity;
};

class ABuildingCustomizableSpawnContainer : public ABuildingContainer
{
	GENERATED_UCLASS_BODY(ABuildingCustomizableSpawnContainer, FortniteGame)
public:

	UPROPERTY(TArray<FFortItemQuantityPair>, SpawnItems)
	TArray<FFortItemQuantityPair> SpawnItems;
};