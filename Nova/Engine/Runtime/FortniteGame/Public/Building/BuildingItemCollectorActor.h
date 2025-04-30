#pragma once
#include "BuildingGameplayActor.h"
#include "Inventory/FortWorldItem.h"
#include "Pawns/FortPlayerPawn.h"

#include "AttributeSet.h"

class FCollectorUnitInfo
{
	GENERATED_USTRUCT_BODY(FCollectorUnitInfo, FortniteGame)
public:

	UPROPERTY_STRUCT(UFortWorldItemDefinition*, InputItem)
	UFortWorldItemDefinition* InputItem;

	UPROPERTY_STRUCT(FScalableFloat, InputCount)
	FScalableFloat InputCount;

	UPROPERTY_STRUCT_BOOL(bUseDefinedOutputItem)
	bool bUseDefinedOutputItem;

	UPROPERTY_STRUCT(UFortWorldItemDefinition*, OutputItem)
	UFortWorldItemDefinition* OutputItem;

	UPROPERTY_STRUCT(TArray<FFortItemEntry>, OutputItemEntry)
	TArray<FFortItemEntry> OutputItemEntry;

	UPROPERTY_STRUCT(FName, OverrideOutputItemLootTierGroupName)
	FName OverrideOutputItemLootTierGroupName;
};

class ABuildingItemCollectorActor : public ABuildingGameplayActor
{
	GENERATED_UCLASS_BODY(ABuildingItemCollectorActor, FortniteGame)
public:

	UPROPERTY(TArray<FCollectorUnitInfo>, ItemCollections)
	TArray<FCollectorUnitInfo> ItemCollections;

	UPROPERTY(uint8, OverrideVendingMachineRarity)
	uint8 OverrideVendingMachineRarity;

	UPROPERTY(UFortWorldItemDefinition*, ActiveInputItem)
	UFortWorldItemDefinition* ActiveInputItem;

	UPROPERTY_BOOL(bUseInstanceLootValueOverrides)
	bool bUseInstanceLootValueOverrides;

	UPROPERTY_BOOL(bCurrentInteractionSuccess)
	bool bCurrentInteractionSuccess;

	UFortWorldItemDefinition* GetCurrentActiveItem();

	void PickupSpawned_Bind();
	void SpawnLoot(AFortPlayerPawn* Pawn);
};