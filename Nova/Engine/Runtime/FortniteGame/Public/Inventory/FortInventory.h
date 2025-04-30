#pragma once
#include "GameFramework/Actor.h"
#include "FortWorldItem.h"

class AFortWeapon;

enum class EFortQuickBars : uint8
{
	Primary = 0,
	Secondary = 1,
	Max_None = 2
};

class FFortItemList : public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY(FFortItemList, FortniteGame)
public:

	UPROPERTY_STRUCT(TArray<FFortItemEntry>, ReplicatedEntries)
	TArray<FFortItemEntry> ReplicatedEntries;

	UPROPERTY_STRUCT(TArray<UFortWorldItem*>, ItemInstances)
	TArray<UFortWorldItem*> ItemInstances;
};

class AFortInventory : public AActor
{
	GENERATED_UCLASS_BODY(AFortInventory, FortniteGame)
public:

	UPROPERTY(FFortItemList, Inventory)
	FFortItemList Inventory;

	static bool IsPrimaryQuickbar(UFortItemDefinition* ItemDefinition);
	bool IsInventoryFull(int Start = 0);

	std::vector<UFortWorldItem*> FindAllInstances(UFortItemDefinition* ItemDef, std::vector<std::function<bool(UFortWorldItem*)>> Conditions);
	std::vector<UFortWorldItem*> GetStackableItems(UFortItemDefinition* ItemDef, int Count);
	std::vector<UFortWorldItem*> GetDroppableItems(UFortItemDefinition* IgnoreDef = nullptr);

	FFortItemEntry* FindItemEntry(FGuid ItemGuid);
	UFortWorldItem* FindItemInstance(FGuid ItemGuid);

	FFortItemEntry* FindItemEntry(UFortItemDefinition* ItemDefinition);
	UFortWorldItem* FindItemInstance(UFortItemDefinition* ItemDefinition);

	bool AddItem(UFortItemDefinition* ItemDef, int Count, bool& NeedsUpdate = *new bool, int LoadedAmmo = 0, bool& WasSwapped = *new bool, bool Force = false);
	bool RemoveItem(FGuid ItemGuid, int Count);

	void Clear();

	void UpdateItem(FGuid ItemGuid, int Count, int LoadedAmmo, AFortWeapon* Weapon = NULL);

	AFortPlayerController* GetOwningPlayer();
	FGuid GetRecentItem();
	FGuid GetPickaxeItem();

	void HandleInventoryLocalUpdate();
	void Update();
};