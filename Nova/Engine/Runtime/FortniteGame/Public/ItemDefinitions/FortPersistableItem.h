#pragma once
#include "FortItem.h"

class UFortPersistableItem : public UFortItem
{
	GENERATED_UCLASS_BODY(UFortPersistableItem, FortniteGame)
public:

	UPROPERTY(UFortItemDefinition*, ItemDefinition)
	UFortItemDefinition* ItemDefinition;

	UPROPERTY(FString, InstanceID)
	FString InstanceID;
};