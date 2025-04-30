#include "Player/FortPlayerController.h"

#include "Pawns/FortPlayerPawn.h"

#include "Player/FortPlayerState.h"

#include "Building/BuildingStructuralSupportSystem.h"
#include "Building/BuildingContainer.h"
#include "Building/FortAthenaSupplyDrop.h"
#include "Building/FortPhysicsBall.h"

#include "Building/BuildingGameplayActorSpawnChip.h"

#include "Athena/Player/FortPlayerStateAthena.h"

#include "Weapons/FortDecoTool.h"
#include "Weapons/FortWeap_EditingTool.h"

#include "Weapons/FortDecoTool_ContextTrap.h"

#include "Kismet/FortKismetLibrary.h"

#include "ItemDefinitions/FortDecoItemDefinition.h"
#include "ItemDefinitions/FortGadgetItemDefinition.h"

#include "ItemDefinitions/AthenaSprayItemDefinition.h"
#include "ItemDefinitions/AthenaDanceItemDefinition.h"
#include "ItemDefinitions/AthenaToyItemDefinition.h"

#include "ItemDefinitions/FortMontageItemDefinitionBase.h"

#include "Athena/Player/FortPlayerControllerAthena.h"

#include "AbilitySystemComponent.h"

#include "GameplayAbility.h"

void AFortPlayerController::HandleWorldInventoryLocalUpdate()
{
	this->ProcessEvent(L"HandleWorldInventoryLocalUpdate");
}

void AFortPlayerController::ClientForceUpdateQuickbar(EFortQuickBars QuickbarToRefresh)
{
	this->ProcessEvent(L"ClientForceUpdateQuickbar", &QuickbarToRefresh);
}

void AFortPlayerController::ClientReturnToMainMenu(FString Reason, bool bForce)
{
	this->ProcessEvent(L"ClientReturnToMainMenu", &Reason);

	if (NetConnection && bForce)
		NetConnection->Close();
}

void AFortPlayerController::ClientActivateSlot(EFortQuickBars InQuickBar, int Slot, float ActivateDelay, bool bUpdatePreviousFocusedSlot, bool bForceExecution)
{
	struct
	{
		EFortQuickBars InQuickBar;
		int Slot;
		float ActivateDelay;
		bool bUpdatePreviousFocusedSlot;
		bool bForceExecution;
	} params(InQuickBar, Slot, ActivateDelay, bUpdatePreviousFocusedSlot, bForceExecution);

	this->ProcessEvent(L"ClientActivateSlot", &params);
}

void AFortPlayerController::ClientEquipItem(FGuid ItemGuid, bool bForceExecution)
{
	struct
	{
		FGuid ItemGuid;
		bool bForceExecution;
	}params(ItemGuid, bForceExecution);

	this->ProcessEvent(L"ClientEquipItem", &params);
}

void AFortPlayerController::AddItemToQuickBars(UFortItemDefinition* ItemDefinition, EFortQuickBars QuickBarType, int SlotIndex)
{
	struct
	{
		UFortItemDefinition* ItemDefinition;
		EFortQuickBars QuickBarType;
		int SlotIndex;
	} params(ItemDefinition, QuickBarType, SlotIndex);

	this->ProcessEvent(L"AddItemToQuickBars", &params);
}

void AFortPlayerController::ClientReportDamagedResourceBuilding(ABuildingSMActor* BuildingSMActor, EFortResourceType PotentialResourceType, int PotentialResourceCount, bool bDestroyed, bool bJustHitWeakspot)
{
	struct
	{
		ABuildingSMActor* BuildingSMActor;
		EFortResourceType PotentialResourceType;
		int PotentialResourceCount;
		bool bJustHitWeakspot;
	} params(BuildingSMActor, PotentialResourceType, PotentialResourceCount, bJustHitWeakspot);

	this->ProcessEvent(L"ClientReportDamagedResourceBuilding", &params);
}

bool AFortPlayerController::IsInAircraft()
{
	bool RetValue;
	this->ProcessEvent(L"IsInAircraft", &RetValue);

	return RetValue;
}

ABuildingSMActor* AFortPlayerController::CreateBuildingActor(const FVector& Location, const FRotator& Rotation, bool bMirrored, UClass* BuildingClass)
{
	if (IsInAircraft())
		return nullptr;

	TArray<ABuildingActor*> ExistingBuilds;

	auto Result = UBuildingStructuralSupportSystem::CanAddBuildingActorClassToGrid(GetWorld(), BuildingClass, Location, Rotation, bMirrored, ExistingBuilds);

	if (Result != EFortStructuralGridQueryResults::CanAdd)
		return nullptr;

	if (auto BuildingActor = GetWorld()->SpawnActor<ABuildingSMActor>(Location, Rotation, BuildingClass))
	{
		BuildingActor->SetMirrored(bMirrored);
		BuildingActor->InitializeKismetSpawnedBuildingActor(BuildingActor, this, true);
		BuildingActor->bPlayerPlaced = true;

		if (auto FortPlayerState = Cast<AFortPlayerStateAthena>(PlayerState))
			BuildingActor->SetTeam(FortPlayerState->TeamIndex);

		for (auto ExistingBuild : ExistingBuilds)
			ExistingBuild->DestroyActor();

		auto ItemDef = UFortKismetLibrary::GetResourceItemDefinition(BuildingActor->ResourceType);

		if (auto ItemEntry = WorldInventory->FindItemEntry(ItemDef))
		{
			if (WorldInventory->RemoveItem(ItemEntry->ItemGuid, 10))
				WorldInventory->Update();
		}

		return BuildingActor;
	}

	return nullptr;
}

AActor* AFortPlayerController::SpawnToyInstance(UClass* ToyClass, FTransform SpawnPosition)
{
	UE_LOG(LogFort, Log, TEXT("SpawnToyInstance: %s"), *ToyClass->GetName());

	return GetWorld()->SpawnActor<AFortPhysicsBall>(SpawnPosition, ToyClass);
}

void AFortPlayerController::ServerExecuteInventoryItem(const FGuid& ItemGuid)
{
	if (IsInAircraft())
		return;

	if (auto ItemEntry = WorldInventory->FindItemEntry(ItemGuid))
	{
		auto Pawn = MyFortPawn;

		if (!Pawn || Pawn->bIsDBNO)
			return;

		if (auto TrapItemDef = Cast<UFortDecoItemDefinition>(ItemEntry->ItemDefinition))
		{
			MyFortPawn->PickUpActor(NULL, TrapItemDef);

			if (MyFortPawn->CurrentWeapon)
				MyFortPawn->CurrentWeapon->ItemEntryGuid = ItemEntry->ItemGuid;

			if (auto DecoToolContext = Cast<AFortDecoTool_ContextTrap>(MyFortPawn->CurrentWeapon))
			{
				DecoToolContext->ContextTrapItemDefinition = ItemEntry->ItemDefinition;
			}
		}
		else if (auto GadgetItem = Cast<UFortGadgetItemDefinition>(ItemEntry->ItemDefinition))
		{
			Pawn->EquipWeaponDefinition(GadgetItem->GetWeaponItemDefinition(), ItemEntry->ItemGuid);
		}
		else
		{
			Pawn->EquipWeaponDefinition(ItemEntry->ItemDefinition, ItemEntry->ItemGuid);
		}
	}
	else
	{
		UE_LOG(LogFort, Log, TEXT("ServerExecuteInventoryItem with invalid ItemGuid, [%i, %i, %i, %i]"), ItemGuid.A, ItemGuid.B, ItemGuid.C, ItemGuid.D);
	}
}

void AFortPlayerController::ServerCreateBuildingActor(const FVector& Location, const FRotator& Rotation, bool bMirrored, UClass* BuildingClass)
{
	if (!MyFortPawn || MyFortPawn->bIsDBNO)
		return;

	CreateBuildingActor(Location, Rotation, bMirrored, BuildingClass);
}

void AFortPlayerController::ServerBeginEditingBuildingActor(ABuildingSMActor* BuildingActorToEdit)
{
	if (IsInAircraft())
		return;

	if (!MyFortPawn || !BuildingActorToEdit)
		return;

	if (MyFortPawn->CurrentWeapon && MyFortPawn->CurrentWeapon->IsA(AFortWeap_BuildingToolBase::StaticClass()))
		return;

	auto FortPlayerState = Cast<AFortPlayerState>(PlayerState);	
	auto EditToolDef = FindObject<UFortItemDefinition>(L"/Game/Items/Weapons/BuildingTools/EditTool.EditTool");

	auto ItemEntry = WorldInventory->FindItemEntry(EditToolDef);

	if (!ItemEntry)
		return;

	if (auto EditTool = Cast<AFortWeap_EditingTool>(MyFortPawn->EquipWeaponDefinition(EditToolDef, ItemEntry->ItemGuid)))
	{
		EditTool->EditActor = BuildingActorToEdit;
		BuildingActorToEdit->SetEditingPlayer(FortPlayerState);
	}
}

void AFortPlayerController::ServerEndEditingBuildingActor(ABuildingSMActor* BuildingActorToStopEditing)
{
	if (IsInAircraft())
		return;

	if (BuildingActorToStopEditing)
	{
		BuildingActorToStopEditing->SetEditingPlayer(NULL);

		if (auto EditTool = Cast<AFortWeap_EditingTool>(MyFortPawn ? MyFortPawn->CurrentWeapon : NULL))
		{
			EditTool->bEditConfirmed = true;
			EditTool->EditActor = NULL;
		}
	}
}

void AFortPlayerController::ServerEditBuildingActor(ABuildingSMActor* BuildingActorToEdit, UClass* NewBuildingClass, int RotationIterations, bool bMirrored)
{
	if (IsInAircraft() || !MyFortPawn || !BuildingActorToEdit || !NewBuildingClass)
		return;

	if (auto BuildingActor = BuildingActorToEdit->ReplaceBuildingActor(EBuildingReplacementType::Edited, NewBuildingClass, 0, RotationIterations, bMirrored, this))
	{
		if (auto FortPlayerState = Cast<AFortPlayerStateAthena>(PlayerState))
			BuildingActor->SetTeam(FortPlayerState->TeamIndex);
	}
}

void AFortPlayerController::ServerRepairBuildingActor(ABuildingSMActor* BuildingActorToRepair)
{
	if (IsInAircraft())
		return;

	if (BuildingActorToRepair == NULL || BuildingActorToRepair->EditingPlayer != NULL)
		return;

	float BuildingHealthPercentLost = 1.0 - BuildingActorToRepair->GetHealthPercent();
	float RepairCostUnrounded = (10 * BuildingHealthPercentLost) * 0.75;
	int RepairCost = FMath::FloorToInt(RepairCostUnrounded > 0 ? RepairCostUnrounded < 1 ? 1 : RepairCostUnrounded : 0);

	auto ItemDef = UFortKismetLibrary::GetResourceItemDefinition(BuildingActorToRepair->ResourceType);

	if (auto ItemEntry = WorldInventory->FindItemEntry(ItemDef))
	{
		RepairCost = FMath::Min(RepairCost, ItemEntry->Count);

		if (WorldInventory->RemoveItem(ItemEntry->ItemGuid, RepairCost))
			WorldInventory->Update();
	}

	BuildingActorToRepair->RepairBuilding(this, RepairCost);
}

void AFortPlayerController::ServerAttemptInteract(AActor* ReceivingActor)
{
	if (IsInAircraft())
		return;

	if (MyFortPawn != NULL)
	{
		float Distance = MyFortPawn->GetDistanceTo(ReceivingActor) / 1000.0f;

		if (Distance > 10.0f)
			return;
	}

	UE_LOG(LogFort, Log, TEXT("ServerAttemptInteract: %s"), *ReceivingActor->GetName());

	if (auto BuildingContainer = Cast<ABuildingContainer>(ReceivingActor))
	{
		if (BuildingContainer->bAlreadySearched)
			return;

		BuildingContainer->SpawnLoot(MyFortPawn);

		BuildingContainer->BounceContainer();
		BuildingContainer->bAlreadySearched = true;
	}

	if (auto ItemCollector = Cast<ABuildingItemCollectorActor>(ReceivingActor))
	{
		if (ItemCollector->bCurrentInteractionSuccess)
			return;

		ItemCollector->SpawnLoot(MyFortPawn);
	}

	if (auto Chip = Cast<ABuildingGameplayActorSpawnChip>(ReceivingActor))
	{
		UE_LOG(LogFort, Log, TEXT("Chip: OwnerPlayerController: %s, OwnerPlayerId: %s, OwnerPlayerState: %s"), 
			*Chip->OwnerPlayerController->GetName(), *Chip->OwnerPlayerId.ToString(), *Chip->OwnerPlayerState->GetName());
	}

	if (auto SupplyDrop = Cast<AFortAthenaSupplyDrop>(ReceivingActor))
	{
		SupplyDrop->SpawnLoot(MyFortPawn);
	}
}

void AFortPlayerController::ServerRemoveInventoryItem(FGuid ItemGuid, int Count, bool bForceRemoveFromQuickBars, bool bForceRemoval)
{
	if (WorldInventory->RemoveItem(ItemGuid, Count))
		WorldInventory->Update();
}

void AFortPlayerController::ServerAttemptInventoryDrop(FGuid ItemGuid, int Count)
{
	if (IsInAircraft())
		return;

	if (Count <= 0)
		return;

	if (auto ItemEntry = WorldInventory->FindItemEntry(ItemGuid))
	{
		if (ItemEntry->Count < Count)
			return;

		for (auto Weapon : MyFortPawn->CurrentWeaponList)
		{
			if (Weapon && Weapon->ItemEntryGuid == ItemEntry->ItemGuid)
			{
				ItemEntry->LoadedAmmo = Weapon->AmmoCount;
				break;
			}
		}

		if (auto Pickup = UFortKismetLibrary::SpawnPickup(GetWorld(), MyFortPawn->GetActorLocation(), ItemEntry->ItemDefinition, ItemEntry->Count, ItemEntry->LoadedAmmo, this, EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset))
		{
			Pickup->ItemEntry.Count = Count;
			Pickup->OnRep_ItemEntry();
		}

		if (WorldInventory->RemoveItem(ItemGuid, Count))
			WorldInventory->Update();
	}
}

void AFortPlayerController::ServerPlayEmoteItem(UFortMontageItemDefinitionBase* EmoteAsset)
{
	if (IsInAircraft() || EmoteAsset == NULL)
		return;

	static auto GenericAbility = LoadObject<UClass>(TEXT("/Game/Abilities/Emotes/GAB_Emote_Generic.GAB_Emote_Generic_C"));
	static auto SprayAbility = LoadObject<UClass>(TEXT("/Game/Abilities/Sprays/GAB_Spray_Generic.GAB_Spray_Generic_C"));

	UClass* AbilityClass = NULL;

	if (EmoteAsset->IsA<UAthenaSprayItemDefinition>() && SprayAbility)
	{
		AbilityClass = SprayAbility;
	}
	else if (EmoteAsset->IsA<UAthenaToyItemDefinition>())
	{
		AbilityClass = NULL;
	}

	if (!AbilityClass)
	{
		AbilityClass = GenericAbility;

		if (auto DanceItem = Cast<UAthenaDanceItemDefinition>(EmoteAsset))
		{
			if (MyFortPawn && DanceItem->GetClassProperty(L"bMovingEmote") != NULL)
			{
				MyFortPawn->bMovingEmote = DanceItem->bMovingEmote;

				if (DanceItem->GetClassProperty(L"bMoveForwardOnly") != NULL)
					MyFortPawn->bMovingEmoteForwardOnly = DanceItem->bMoveForwardOnly;

				if (DanceItem->GetClassProperty(L"WalkForwardSpeed") != NULL)
					MyFortPawn->EmoteWalkSpeed = DanceItem->WalkForwardSpeed;
			}
		}
	}

	auto FortPlayerState = Cast<AFortPlayerState>(PlayerState);

	if (FortPlayerState && AbilityClass)
	{
		FortPlayerState->AbilitySystemComponent->GiveAbilityAndActivateOnce(AbilityClass, EmoteAsset);
	}
}

void AFortPlayerController::ServerSuicide()
{
	if (MyFortPawn)
	{
		MyFortPawn->ForceKill({}, this, NULL);
	}
}

void AFortPlayerController::ServerReturnToMainMenu()
{
	this->ReturnToMainMenu();
}

void AFortPlayerController::ReturnToMainMenu()
{
	this->ClientReturnToMainMenu(L"", false);
}

bool AFortPlayerController::GetPlayerViewpoint(FVector& OutLocation, FRotator& OutRotation)
{
	if (StateName == NAME_Spectating)
	{
		OutLocation = LastSpectatorSyncLocation;
		OutRotation = LastSpectatorSyncRotation;

		return true;
	}

	if (MyFortPawn)
	{
		if (auto Vehicle = MyFortPawn->GetVehicle())
		{
			OutLocation = Vehicle->GetActorLocation();
			OutRotation = GetControlRotation();

			return true;
		}

		OutLocation = MyFortPawn->GetActorLocation();
		OutRotation = GetControlRotation();

		return true;
	}

	return false;
}

FFortAthenaLoadout& AFortPlayerController::GetCosmeticLoadout()
{
	return GetClassField(L"CustomizationLoadout") ? CustomizationLoadout : CosmeticLoadoutPC;
}

void AFortPlayerController::HandleAthenaProfileUpdate()
{
	bHasServerFinishedLoading = true;
}