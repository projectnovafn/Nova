#include "Kismet/FortKismetLibrary.h"
#include "Athena/FortGameModeAthena.h"
#include "Athena/FortGameStateAthena.h"
#include "Engine/DataTable.h"
#include "Building/BuildingContainer.h"
#include "Engine/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

static FFortLootTierData* GetLootTierData(std::vector<FFortLootTierData*>& LootTierData)
{
    float TotalWeight = 0;

    for (auto Item : LootTierData)
        TotalWeight += Item->Weight;

    float RandomNumber = UKismetMathLibrary::RandomFloatInRange(0, TotalWeight);

    FFortLootTierData* SelectedItem = nullptr;

    for (auto Item : LootTierData)
    {
        if (RandomNumber <= Item->Weight)
        {
            SelectedItem = Item;
            break;
        }

        RandomNumber -= Item->Weight;
    }

    if (!SelectedItem)
        return GetLootTierData(LootTierData);

    return SelectedItem;
}

static FFortLootPackageData* GetLootPackage(std::vector<FFortLootPackageData*>& LootPackages)
{
    float TotalWeight = 0;

    for (auto Item : LootPackages)
        TotalWeight += Item->Weight;

    float RandomNumber = UKismetMathLibrary::RandomFloatInRange(0, TotalWeight);

    FFortLootPackageData* SelectedItem = nullptr;

    for (auto Item : LootPackages)
    {
        if (RandomNumber <= Item->Weight)
        {
            SelectedItem = Item;
            break;
        }

        RandomNumber -= Item->Weight;
    }

    if (!SelectedItem)
        return GetLootPackage(LootPackages);

    return SelectedItem;
}

AActor* UFortKismetLibrary::GetClosestActorFromArray(AActor* Actor, TArray<AActor*> Actors)
{
    struct
    {
		AActor* Actor;
		TArray<AActor*> Actors;
        AActor* ReturnValue;
    } params(Actor, Actors);

    GetDefaultObject<UFortKismetLibrary>()->ProcessEvent(L"GetClosestActorFromArray", &params);

    return params.ReturnValue;
}

AFortPickupAthena* UFortKismetLibrary::SpawnPickup(UWorld* World, FVector Location, UFortItemDefinition* ItemDefinition, int Count, int LoadedAmmo, AFortPlayerController* Instigator, EFortPickupSourceTypeFlag InPickupSourceTypeFlags, EFortPickupSpawnSource InPickupSpawnSource)
{
    auto Pawn = Instigator ? Instigator->MyFortPawn : NULL;

    auto Pickup = World->SpawnActor<AFortPickupAthena>(Location);

    if (!Pickup)
    {
        return NULL;
    }

    if (auto WeaponDef = Cast<UFortWeaponItemDefinition>(ItemDefinition))
        LoadedAmmo = LoadedAmmo == -1 ? GetClipSize(WeaponDef) : LoadedAmmo;

    if (Count >= ItemDefinition->MaxStackSize)
        Count = ItemDefinition->MaxStackSize;

    Pickup->PawnWhoDroppedPickup = Pawn;

    if (Pawn && InPickupSourceTypeFlags == EFortPickupSourceTypeFlag::Player && InPickupSpawnSource == EFortPickupSpawnSource::Unset)
    {
        Location += Pawn->GetActorForwardVector() * 500.0f;
    }

    auto& ItemEntry = Pickup->ItemEntry;
    ItemEntry.ItemDefinition = ItemDefinition;
    ItemEntry.Count = Count;
    ItemEntry.LoadedAmmo = LoadedAmmo;

	Pickup->OnRep_ItemEntry();

    Pickup->TossPickup(Location, NULL, 0, true, InPickupSourceTypeFlags, InPickupSpawnSource);

    Pickup->SetReplicateMovement(true);
    Pickup->OnRep_ReplicateMovement();
    Pickup->MovementComponent = UGameplayStatics::SpawnObject<UProjectileMovementComponent>(Pickup);

	return Pickup;
}

void UFortKismetLibrary::GiveItemToInventoryOwner(AFortPlayerController* InventoryOwner, UFortWorldItemDefinition* ItemDefinition, int NumberToGive, bool bNotifyPlayer, int ItemLevel, int PickupInstigatorHandle)
{
    auto WeaponDefinition = Cast<UFortWeaponItemDefinition>(ItemDefinition);

    if (!InventoryOwner || !ItemDefinition || !NumberToGive)
        return;

    if (auto WorldInventory = InventoryOwner->WorldInventory)
    {
        bool bNeedsUpdate;
        WorldInventory->AddItem(ItemDefinition, NumberToGive, bNeedsUpdate, UFortKismetLibrary::GetClipSize(WeaponDefinition));
        
        if (bNeedsUpdate)
            WorldInventory->Update();
    }
}

UFortItemDefinition* UFortKismetLibrary::GetResourceItemDefinition(EFortResourceType ResourceType)
{
	struct
	{
        EFortResourceType ResourceType;
		UFortItemDefinition* ReturnValue;
	} params(ResourceType);

	GetDefaultObject<UFortKismetLibrary>()->ProcessEvent(L"K2_GetResourceItemDefinition", &params);

	return params.ReturnValue;
}

int32 UFortKismetLibrary::GetClipSize(UFortWeaponItemDefinition* WeaponDef)
{
    if (!WeaponDef)
        return 0;

    auto Table = WeaponDef->WeaponStatHandle.DataTable;

    if (!Table)
        return 0;

    FFortRangedWeaponStats* Row = NULL;

    for (auto&& Pair : Table->GetAs<FFortRangedWeaponStats>())
    {
        if (Pair.Key == WeaponDef->WeaponStatHandle.RowName)
            return Pair.Value->ClipSize;
    }
    
    return 0;
}

TArray<UObject*> UFortKismetLibrary::JonLHack_GetAllObjectsOfClassFromPath(FString Path, UClass* Class)
{
    struct
    {
        FString Path;
        UClass* Class;
        TArray<UObject*> ReturnValue;
    } params(Path, Class);

    GetDefaultObject<UFortKismetLibrary>()->ProcessEvent(L"JonLHack_GetAllObjectsOfClassFromPath", &params);

    return params.ReturnValue;
}

FVector UFortKismetLibrary::FindStaticGroundLocationAt(UWorld* World, FVector InLocation, AActor* IgnoreActor, float TraceStartZ, float TraceEndZ)
{
    struct
    {
        UWorld* World;
        FVector InLocation;
        AActor* IgnoreActor;
        float TraceStartZ;
        float TraceEndZ;
        FVector ReturnValue;
    } params(World, InLocation, IgnoreActor, TraceStartZ, TraceEndZ);

    GetDefaultObject<UFortKismetLibrary>()->ProcessEvent(L"FindStaticGroundLocationAt", &params);

    return params.ReturnValue;
}

bool UFortKismetLibrary::UpdatePlayerCustomCharacterPartsVisualization(AFortPlayerState* PlayerState)
{
    if (GetDefaultObject<UFortKismetLibrary>()->GetClassField(L"UpdatePlayerCustomCharacterPartsVisualization"))
    {
        GetDefaultObject<UFortKismetLibrary>()->ProcessEvent(L"UpdatePlayerCustomCharacterPartsVisualization", &PlayerState);

        return true;
    }

    return false;
}

AFortProjectileBase* UFortKismetLibrary::SpawnProjectile(UClass* ProjectileClass, AActor* RequestedBy, FVector SpawnLocation, FRotator SpawnRotation, FRotator SpawnDirection, FFortGameplayEffectContainerSpec EffectContainerSpecToApplyOnHit, FFortGameplayEffectContainerSpec EffectContainerSpecToApplyOnExplode, bool bUseDefaultPhysics, AActor* HomingTarget, float InitialSpeed, float GravityScale, float ChargePercent)
{
    struct
    {
        UClass* ProjectileClass;
        AActor* RequestedBy; 
        FVector SpawnLocation; 
        FRotator SpawnRotation; 
        FRotator SpawnDirection; 
        FFortGameplayEffectContainerSpec EffectContainerSpecToApplyOnHit; 
        FFortGameplayEffectContainerSpec EffectContainerSpecToApplyOnExplode; 
        bool bUseDefaultPhysics; 
        AActor* HomingTarget; 
        float InitialSpeed; 
        float GravityScale; 
        float ChargePercent;
        AFortProjectileBase* ReturnValue;
    } params(ProjectileClass, RequestedBy, SpawnLocation, SpawnRotation, SpawnDirection, EffectContainerSpecToApplyOnHit, EffectContainerSpecToApplyOnExplode, bUseDefaultPhysics, HomingTarget, InitialSpeed, GravityScale, ChargePercent);

    GetDefaultObject<UFortKismetLibrary>()->ProcessEvent(L"SpawnProjectile", &params);

    return params.ReturnValue;
}

std::vector<FFortItemEntry*> UFortKismetLibrary::PickLootDrops(UWorld* World, FName TierGroupName, int ForcedLootTier, int Count)
{
    std::vector<FFortItemEntry*> LootDrops;

    if (Count >= 10)
        return LootDrops;

    auto GameState = Cast<AFortGameStateAthena>(World->GameState);
    auto Playlist = GameState->GetCurrentPlaylist();

    auto MainLTD = Playlist->LootTierData.IsValid() ? Playlist->LootTierData.Get() : 
        LoadObject<UDataTable>(L"/Game/Items/Datatables/AthenaLootTierData_Client.AthenaLootTierData_Client");
    auto MainLP = Playlist->LootPackages.IsValid() ? Playlist->LootPackages.Get() :
        LoadObject<UDataTable>(L"/Game/Items/Datatables/AthenaLootPackages_Client.AthenaLootPackages_Client");

    std::vector<FFortLootTierData*> TierGroupLTDs;

	for (auto& LootTier : MainLTD->GetAs<FFortLootTierData>())
	{
		auto TierData = LootTier.Value;

		if (TierGroupName == TierData->TierGroup && TierData->Weight != 0 && (ForcedLootTier == -1 || ForcedLootTier == TierData->LootTier))
		{
			TierGroupLTDs.emplace_back(TierData);
		}
	}

    if (TierGroupLTDs.size() == 0)
    {
        return LootDrops;
    }

    auto ChosenRowLootTierData = GetLootTierData(TierGroupLTDs);

    if (!ChosenRowLootTierData)
        return LootDrops;

    if (ChosenRowLootTierData->NumLootPackageDrops <= 0)
        return PickLootDrops(World, TierGroupName, ForcedLootTier, ++Count);

    if (ChosenRowLootTierData->LootPackageCategoryMinArray.Num() != ChosenRowLootTierData->LootPackageCategoryWeightArray.Num() ||
        ChosenRowLootTierData->LootPackageCategoryMinArray.Num() != ChosenRowLootTierData->LootPackageCategoryMaxArray.Num())
        return PickLootDrops(World, TierGroupName, ForcedLootTier, ++Count);

    int MinimumLootDrops = 0;
    float NumLootPackageDrops = FMath::FloorToFloat(ChosenRowLootTierData->NumLootPackageDrops);

    if (UKismetSystemLibrary::GetEngineVersionNum() >= 4.24)
        NumLootPackageDrops += 1;

    for (int i = 0; i < ChosenRowLootTierData->LootPackageCategoryMinArray.Num(); i++)
    {
        if (ChosenRowLootTierData->LootPackageCategoryMinArray[i] > 0)
        {
            MinimumLootDrops += ChosenRowLootTierData->LootPackageCategoryMinArray[i];
        }
    }

    int SumLootPackageCategoryWeightArray = 0;

    for (int i = 0; i < ChosenRowLootTierData->LootPackageCategoryWeightArray.Num(); i++)
    {
        auto CategoryWeight = ChosenRowLootTierData->LootPackageCategoryWeightArray[i];

        if (CategoryWeight > 0)
        {
            auto CategoryMaxArray = ChosenRowLootTierData->LootPackageCategoryMaxArray[i];

            if (CategoryMaxArray < 0)
            {
                SumLootPackageCategoryWeightArray += CategoryWeight;
            }
        }
    }

    int SumLootPackageCategoryMinArray = 0;

    for (int i = 0; i < ChosenRowLootTierData->LootPackageCategoryMinArray.Num(); i++)
    {
        auto CategoryWeight = ChosenRowLootTierData->LootPackageCategoryMinArray[i];

        if (CategoryWeight > 0)
        {
            auto CategoryMaxArray = ChosenRowLootTierData->LootPackageCategoryMaxArray[i];

            if (CategoryMaxArray < 0)
            {
                SumLootPackageCategoryMinArray += CategoryWeight;
            }
        }
    }

    if (SumLootPackageCategoryWeightArray > SumLootPackageCategoryMinArray)
        return PickLootDrops(World, TierGroupName, ForcedLootTier, ++Count);

    std::vector<FFortLootPackageData*> TierGroupLPs;

    for (auto& CurrentLP : MainLP->GetAs<FFortLootPackageData>())
    {
        auto LootPackage = CurrentLP.Value;

        if (!LootPackage)
            continue;

        if (LootPackage->LootPackageID == ChosenRowLootTierData->LootPackage && LootPackage->Weight != 0)
        {
            TierGroupLPs.emplace_back(LootPackage);
        }
    }

    auto ChosenLootPackageName = ChosenRowLootTierData->LootPackage.ToString();

    if (ChosenLootPackageName.Find(L".Empty") != INDEX_NONE)
    {
        return PickLootDrops(World, TierGroupName, ForcedLootTier, ++Count);
    }

    bool bIsWorldList = ChosenLootPackageName.Find(L"WorldList") != INDEX_NONE;

    LootDrops.reserve(NumLootPackageDrops);

    for (float i = 0; i < NumLootPackageDrops; i++)
    {
        if (i >= TierGroupLPs.size())
            break;

        auto TierGroupLP = TierGroupLPs[i];
        auto TierGroupLPStr = TierGroupLP->LootPackageCall;

        if (TierGroupLPStr.Find(L".Empty") != INDEX_NONE)
        {
            NumLootPackageDrops++;
            continue;
        }

        std::vector<FFortLootPackageData*> lootPackageCalls;

        if (bIsWorldList)
        {
            for (int j = 0; j < TierGroupLPs.size(); j++)
            {
                auto& CurrentLP = TierGroupLPs[j];

                if (CurrentLP->Weight != 0)
                    lootPackageCalls.emplace_back(CurrentLP);
            }
        }
        else
        {
            for (auto& CurrentLP : MainLP->GetAs<FFortLootPackageData>())
            {
                auto LootPackage = CurrentLP.Value;

                if (LootPackage->LootPackageID.ToString() == TierGroupLPStr && LootPackage->Weight != 0)
                {
                    lootPackageCalls.emplace_back(LootPackage);
                }
            }
        }

        if (lootPackageCalls.size() == 0)
        {
            NumLootPackageDrops++;
            continue;
        }

        auto LootPackageCall = GetLootPackage(lootPackageCalls);

        if (!LootPackageCall)
            continue;

        auto ItemDef = LootPackageCall->ItemDefinition.Get();

        if (!ItemDef)
        {
            NumLootPackageDrops++;
            continue;
        }

        FFortItemEntry* LootDropEntry = FFortItemEntry::Allocate();

        LootDropEntry->ItemDefinition = ItemDef;
        LootDropEntry->LoadedAmmo = GetClipSize(Cast<UFortWeaponItemDefinition>(ItemDef));
        LootDropEntry->Count = LootPackageCall->Count;

        LootDrops.emplace_back(LootDropEntry);
    }

	return LootDrops;
}

void UFortKismetLibrary::SpawnLlamas(UWorld* World)
{
    /*auto GameState = Cast<AFortGameStateAthena>(World->GameState);
    auto MapInfo = GameState->MapInfo;
    auto LlamaQuantity = MapInfo->GetLlamaQuantity();

    for (int i = 0; i < LlamaQuantity; i++)
    {
        auto Location = MapInfo->PickSupplyDropLocation(FVector(1, 1, 10000), 100000);

        if (Location.IsEmpty())
            continue;

        auto Llama = GetWorld()->SpawnActor<AFortAthenaSupplyDrop>(Location, {}, MapInfo->LlamaClass, false);
        auto GroundLocation = Llama->FindGroundLocationAt(Location);
        auto Transform = Llama->GetActorTransform();

        Transform.Translation = GroundLocation;
        Transform.Scale3D = { 1,1,1 };

        UGameplayStatics::FinishSpawningActor(Llama, Transform);
    }*/
}

void UFortKismetLibrary::FillVendingMachine(UWorld* World, ABuildingItemCollectorActor* ItemCollector)
{
    if (UKismetSystemLibrary::GetEngineVersionNum() <= 4.22 || UKismetSystemLibrary::GetEngineVersionNum() >= 4.25)
    {
        ItemCollector->DestroyActor();
        return;
    }

    auto LootTier = UKismetMathLibrary::RandomIntegerInRange(0, 4);

    for (int i = 0; i < 3; i++)
    {
        auto& ItemCollection = ItemCollector->ItemCollections[i];
        auto LootDrops = UFortKismetLibrary::PickLootDrops(World, L"Loot_AthenaVending", LootTier);

        if (ItemCollection.OutputItemEntry.Num() > 0)
        {
            ItemCollection.OutputItemEntry.Reset();
            ItemCollection.OutputItem = nullptr;
        }

        for (auto& LootDrop : LootDrops)
        {
            if (AFortInventory::IsPrimaryQuickbar(LootDrop->ItemDefinition))
            {
                if (auto WorldItemDef = Cast<UFortWorldItemDefinition>(LootDrop->ItemDefinition))
                    ItemCollection.OutputItem = WorldItemDef;
            }
        }

        if (!ItemCollection.OutputItem)
            return FillVendingMachine(World, ItemCollector);

        for (auto& LootDrop : LootDrops)
            UKismetArrayLibrary::Array_Add(ItemCollection.OutputItemEntry, FCollectorUnitInfo::StaticStruct()->GetProperty(L"OutputItemEntry"), *LootDrop);

        ItemCollector->bUseInstanceLootValueOverrides = true;
        ItemCollector->OverrideVendingMachineRarity = ItemCollection.OutputItem->Rarity;
    }
}

void UFortKismetLibrary::SpawnDynamicItems(UWorld* World)
{
    auto SpawnIsland_FloorLoot = FindObject<UClass>(L"/Game/Athena/Environments/Blueprints/Tiered_Athena_FloorLoot_Warmup.Tiered_Athena_FloorLoot_Warmup_C");
    auto BRIsland_FloorLoot = FindObject<UClass>(L"/Game/Athena/Environments/Blueprints/Tiered_Athena_FloorLoot_01.Tiered_Athena_FloorLoot_01_C");
    
    auto SpawnIsland_FloorLoot_Actors = UGameplayStatics::GetAllActorsOfClass<ABuildingContainer>(World, SpawnIsland_FloorLoot);
    auto BRIsland_FloorLoot_Actors = UGameplayStatics::GetAllActorsOfClass<ABuildingContainer>(World, BRIsland_FloorLoot);
    auto ItemCollector_Actors = UGameplayStatics::GetAllActorsOfClass<ABuildingItemCollectorActor>(World);

    static FName SpawnIslandTierGroup = L"Loot_AthenaFloorLoot_Warmup";
    static FName BRIslandTierGroup = L"Loot_AthenaFloorLoot";
    
    for (auto& Container : SpawnIsland_FloorLoot_Actors)
    {
        for (auto& LootDrop : UFortKismetLibrary::PickLootDrops(World, SpawnIslandTierGroup))
            SpawnPickup(World, Container->GetActorLocation() + FVector(0, 0, 100), LootDrop->ItemDefinition, LootDrop->Count, LootDrop->LoadedAmmo, NULL, EFortPickupSourceTypeFlag::FloorLoot, EFortPickupSpawnSource::Unset);
    }

    for (auto& Container : BRIsland_FloorLoot_Actors)
    {
        for (auto& LootDrop : UFortKismetLibrary::PickLootDrops(World, BRIslandTierGroup))
            SpawnPickup(World, Container->GetActorLocation() + FVector(0, 0, 100), LootDrop->ItemDefinition, LootDrop->Count, LootDrop->LoadedAmmo, NULL, EFortPickupSourceTypeFlag::FloorLoot, EFortPickupSpawnSource::Unset);
    }

    for (auto& ItemCollector : ItemCollector_Actors)
    {
        FillVendingMachine(World, ItemCollector);
    }

    SpawnLlamas(World);
}
