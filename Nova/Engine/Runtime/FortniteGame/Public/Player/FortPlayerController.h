#pragma once

#include "GameFramework/PlayerController.h"
#include "Inventory/FortInventory.h"
#include "Pawns/FortPlayerPawn.h"

#include "Building/BuildingSMActor.h"

#include "ItemDefinitions/AthenaCharacterItemDefinition.h"
#include "ItemDefinitions/AthenaPickaxeItemDefinition.h"
#include "ItemDefinitions/AthenaBackpackItemDefinition.h"

#include "Interfaces/FortInventoryOwnerInterface.h"

#include "FortRegisteredPlayerInfo.h"

#include "Mcp/McpProfileGroup.h"
#include "Athena/Online/Mcp/FortMcpProfileCreative.h"

class AFortPlayerPawn;
class ABuildingSMActor;

class UFortMontageItemDefinitionBase;

class FFortAthenaLoadout
{
	GENERATED_USTRUCT_BODY(FFortAthenaLoadout, FortniteGame)
public:

	UPROPERTY_STRUCT(UAthenaBackpackItemDefinition*, Backpack)
	UAthenaBackpackItemDefinition* Backpack;

	UPROPERTY_STRUCT(UAthenaCharacterItemDefinition*, Character)
	UAthenaCharacterItemDefinition* Character;

	UPROPERTY_STRUCT(UAthenaPickaxeItemDefinition*, Pickaxe)
	UAthenaPickaxeItemDefinition* Pickaxe;
};

class AFortBroadcastRemoteClientInfo : public AActor
{
	GENERATED_UCLASS_BODY(AFortBroadcastRemoteClientInfo, FortniteGame)

public:

	UPROPERTY(EFortResourceType, RemoteBuildingMaterial)
	EFortResourceType RemoteBuildingMaterial;

	UPROPERTY(UClass*, RemoteBuildableClass)
	UClass* RemoteBuildableClass;
};

class AFortPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY(AFortPlayerController, FortniteGame)
public:

	UPROPERTY_NOTIFY(bool, bHasServerFinishedLoading)
	bool bHasServerFinishedLoading;

	UPROPERTY(bool, bHasClientFinishedLoading)
	bool bHasClientFinishedLoading;

	UPROPERTY(AFortInventory*, WorldInventory)
	AFortInventory* WorldInventory;

	UPROPERTY(AFortPlayerPawn*, MyFortPawn)
	AFortPlayerPawn* MyFortPawn;

	UPROPERTY(FFortAthenaLoadout, CustomizationLoadout)
	FFortAthenaLoadout CustomizationLoadout;

	UPROPERTY(FFortAthenaLoadout, CosmeticLoadoutPC)
	FFortAthenaLoadout CosmeticLoadoutPC;

	UPROPERTY(UMcpProfileGroup*, McpProfileGroup)
	UMcpProfileGroup* McpProfileGroup;

	UPROPERTY(AFortBroadcastRemoteClientInfo*, BroadcastRemoteClientInfo)
	AFortBroadcastRemoteClientInfo* BroadcastRemoteClientInfo;

	UPROPERTY(UFortRegisteredPlayerInfo*, MyPlayerInfo)
	UFortRegisteredPlayerInfo* MyPlayerInfo;

	UPROPERTY(UFortMcpProfileCreative*, CreativeModeProfile)
	UFortMcpProfileCreative* CreativeModeProfile;

	UPROPERTY_BOOL(bTryPickupSwap)
	bool bTryPickupSwap;

	void HandleWorldInventoryLocalUpdate();

	void ClientForceUpdateQuickbar(EFortQuickBars QuickbarToRefresh);
	void ClientActivateSlot(EFortQuickBars InQuickBar, int Slot, float ActivateDelay, bool bUpdatePreviousFocusedSlot, bool bForceExecution);
	void ClientEquipItem(struct FGuid ItemGuid, bool bForceExecution);

	void AddItemToQuickBars(UFortItemDefinition* ItemDefinition, EFortQuickBars QuickBarType, int SlotIndex);

	void ServerExecuteInventoryItem(const FGuid& ItemGuid);

	void ServerCreateBuildingActor(const FVector& Location, const FRotator& Rotation, bool bMirrored, UClass* BuildingClass);

	void ServerBeginEditingBuildingActor(ABuildingSMActor* BuildingActorToEdit);
	void ServerEndEditingBuildingActor(ABuildingSMActor* BuildingActorToStopEditing);

	void ServerEditBuildingActor(ABuildingSMActor* BuildingActorToEdit, UClass* NewBuildingClass, int RotationIterations, bool bMirrored);

	void ServerRepairBuildingActor(ABuildingSMActor* BuildingActorToRepair);

	void ServerAttemptInteract(AActor* ReceivingActor);

	void ServerRemoveInventoryItem(FGuid ItemGuid, int Count, bool bForceRemoveFromQuickBars, bool bForceRemoval);
	void ServerAttemptInventoryDrop(FGuid ItemGuid, int Count);

	void ServerPlayEmoteItem(UFortMontageItemDefinitionBase* EmoteAsset);

	void ServerSuicide();

	void ServerReturnToMainMenu();
	void ReturnToMainMenu();
	void ClientReturnToMainMenu(FString Reason, bool bForce = true);

	bool GetPlayerViewpoint(FVector& out_Location, FRotator& out_Rotation);

	FFortAthenaLoadout& GetCosmeticLoadout();

	void ClientReportDamagedResourceBuilding(ABuildingSMActor* BuildingSMActor, EFortResourceType PotentialResourceType, int PotentialResourceCount, bool bDestroyed, bool bJustHitWeakspot);

	bool IsInAircraft();

	ABuildingSMActor* CreateBuildingActor(const FVector& Location, const FRotator& Rotation, bool bMirrored, UClass* BuildingClass);

	AActor* SpawnToyInstance(UClass* ToyClass, FTransform SpawnPosition);

	void HandleAthenaProfileUpdate();
};