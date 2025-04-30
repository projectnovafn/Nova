#pragma once
#include "ItemDefinitions/FortItem.h"
#include "Engine/NetSerialization.h"

class AFortPlayerController;

enum EFortItemEntryState : uint8
{
	NoneState = 0,
	NewItemCount = 1,
	ShouldShowItemToast = 2,
	DurabilityInitialized = 3,
	DoNotShowSpawnParticles = 4,
	FromRecoveredBackpack = 5,
	FromGift = 6,
	PendingUpgradeCriteriaProgress = 7,
	OwnerBuildingHandle = 8,
	FromDroppedPickup = 9,
	JustCrafted = 10,
	CraftAndSlotTarget = 11,
	GenericAttributeValueSet = 12
};

class FFortItemEntryStateValue
{
	GENERATED_USTRUCT_BODY(FFortItemEntryStateValue, FortniteGame)

public:
	UPROPERTY_STRUCT(int, IntValue)
	int IntValue;

	UPROPERTY_STRUCT(FName, NameValue)
	FName NameValue;

	UPROPERTY_STRUCT(EFortItemEntryState, StateType)
	EFortItemEntryState StateType;
};

class FFortItemEntry : public FFastArraySerializerItem
{
	GENERATED_USTRUCT_BODY(FFortItemEntry, FortniteGame)
	USTRUCT_COPYABLE(FFortItemEntry)

public:

	UPROPERTY_STRUCT(int, Count)
	int Count;

	UPROPERTY_STRUCT(UFortItemDefinition*, ItemDefinition)
	UFortItemDefinition* ItemDefinition;

	UPROPERTY_STRUCT(int, LoadedAmmo)
	int LoadedAmmo;

	UPROPERTY_STRUCT(FGuid, ItemGuid)
	FGuid ItemGuid;

	UPROPERTY_STRUCT(TArray<FFortItemEntryStateValue>, StateValues)
	TArray<FFortItemEntryStateValue> StateValues;

	UPROPERTY_STRUCT(bool, bIsDirty)
	bool bIsDirty;

	inline bool operator==(const FFortItemEntry& Other)
	{
		return ItemGuid == const_cast<FFortItemEntry&>(Other).ItemGuid;
	}

	inline bool operator!=(const FFortItemEntry& Other)
	{
		return ItemGuid != const_cast<FFortItemEntry&>(Other).ItemGuid;
	}
};

class UFortWorldItemDefinition : public UFortItemDefinition
{
	GENERATED_UCLASS_BODY(UFortWorldItemDefinition, FortniteGame)
public:

};

class UFortWorldItem : public UFortItem
{
	GENERATED_UCLASS_BODY(UFortWorldItem, FortniteGame)
public:

	UPROPERTY(FFortItemEntry, ItemEntry)
	FFortItemEntry ItemEntry;

	void SetOwner(AFortPlayerController* PlayerController);
};