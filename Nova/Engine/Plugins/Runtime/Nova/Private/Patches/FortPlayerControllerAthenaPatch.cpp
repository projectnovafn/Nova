 #include "Patches/FortPlayerControllerAthenaPatch.h"
#include "KismetDetoursLibrary.h"

#include "Components/FortControllerComponent_Aircraft.h"

static void (*_ServerAcknowledgePossession)(AFortPlayerControllerAthena*, APawn*);
static void (*_ServerExecuteInventoryItem)(AFortPlayerControllerAthena*, FGuid);
static void (*_execServerCreateBuildingActor)(AFortPlayerControllerAthena*, FFrame&);
static void (*_execServerAttemptInteract)(AFortPlayerControllerAthena*, FFrame&);
static void (*_ServerEditBuildingActor)(AFortPlayerControllerAthena*, ABuildingSMActor*, UClass*, int, bool);
static void (*_ServerBeginEditingBuildingActor)(AFortPlayerControllerAthena*, ABuildingSMActor*);
static void (*_ServerEndEditingBuildingActor)(AFortPlayerControllerAthena*, ABuildingSMActor*);
static void (*_ServerRepairBuildingActor)(AFortPlayerControllerAthena*, ABuildingSMActor*);
static void (*_ServerRemoveInventoryItem)(AFortPlayerControllerAthena*, FGuid, int, bool, bool);
static void (*_ServerAttemptInventoryDrop)(AFortPlayerControllerAthena*, FGuid, int);
static void (*_ServerSuicide)(AFortPlayerControllerAthena*);
static void (*_ServerReturnToMainMenu)(AFortPlayerControllerAthena*);
static void (*_ServerPlayEmoteItem)(AFortPlayerControllerAthena*, UFortMontageItemDefinitionBase*);
static void (*_ServerChangeName)(AFortPlayerControllerAthena*, FString);
static void (*_EnterAircraft)(AFortPlayerControllerAthena*, AFortAthenaAircraft*);
static void (*_ExitAircraft)(AFortPlayerControllerAthena*);
static void (*_GetPlayerViewpoint)(AFortPlayerControllerAthena*, FVector&, FRotator&);
static void (*_ClientOnPawnDied)(AFortPlayerControllerAthena*, FFortPlayerDeathReport&);
static void (*_execEndGhostMode)(AFortPlayerControllerAthena*, FFrame&);
static void (*_execServerGiveCreativeItem)(AFortPlayerControllerAthena*, FFrame&);
static bool (*_IsTeamDead)(AFortPlayerControllerAthena*);
static void (*_ServerAttemptAircraftJump)(AFortPlayerControllerAthena*, FRotator);

static void (*_ServerTeleportToPlaygroundLobbyIsland)(AFortPlayerControllerAthena*);
static void (*_ServerTeleportToPlaygroundIslandDock)(AFortPlayerControllerAthena*);

static AActor* (*_SpawnToyInstance)(AFortPlayerControllerAthena*, FFrame&, AActor**);
static TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* (*_FindTeamMembersPC)(AFortPlayerControllerAthena*);

static void (*_execServerStartMinigame)(AFortPlayerControllerAthena*, FFrame&);
static void (*_ServerCreativeSetFlightSpeedIndex)(AFortPlayerControllerAthena*, int32);
static void (*_ServerCreativeSetFlightSprint)(AFortPlayerControllerAthena*, bool);
static void (*_ServerUpdateActorOptions)(AFortPlayerControllerAthena*, AActor*, TArray<FString>&, TArray<FString>&);

static bool (*_ServerLoadPlotForPortal_Validate)(AFortPlayerControllerAthena*, AFortAthenaCreativePortal*, FString);
static void (*_ServerLoadPlotForPortal)(AFortPlayerControllerAthena*, AFortAthenaCreativePortal*, FString);

UDetour* EnterAircraftDetour = NULL;
UDetour* ExitAircraftDetour = NULL;
UDetour* GetPlayerViewpointDetour = NULL;
UDetour* ClientOnPawnDiedDetour = NULL;
UDetour* IsTeamDeadDetour = NULL;
UDetour* FindTeamMembersPCDetour = NULL;

void AFortPlayerControllerAthenaPatch::ServerAcknowledgePossessionHook(AFortPlayerControllerAthena* PlayerController, APawn* P)
{
	PlayerController->ServerAcknowledgePossession(P);
}

void AFortPlayerControllerAthenaPatch::ServerExecuteInventoryItemHook(AFortPlayerControllerAthena* PlayerController, FGuid ItemGuid)
{
	PlayerController->ServerExecuteInventoryItem(ItemGuid);
}

void AFortPlayerControllerAthenaPatch::ServerEditBuildingActorHook(AFortPlayerControllerAthena* PlayerController, ABuildingSMActor* BuildingActorToEdit, UClass* NewBuildingClass, int RotationIterations, bool bMirrored)
{
	PlayerController->ServerEditBuildingActor(BuildingActorToEdit, NewBuildingClass, RotationIterations, bMirrored);
}

void AFortPlayerControllerAthenaPatch::ServerBeginEditingBuildingActorHook(AFortPlayerControllerAthena* PlayerController, ABuildingSMActor* BuildingActorToEdit)
{
	PlayerController->ServerBeginEditingBuildingActor(BuildingActorToEdit);
}

void AFortPlayerControllerAthenaPatch::ServerEndEditingBuildingActorHook(AFortPlayerControllerAthena* PlayerController, ABuildingSMActor* BuildingActorToStopEditing)
{
	PlayerController->ServerEndEditingBuildingActor(BuildingActorToStopEditing);
}

void AFortPlayerControllerAthenaPatch::ServerRepairBuildingActorHook(AFortPlayerControllerAthena* PlayerController, ABuildingSMActor* BuildingActorToRepair)
{
	PlayerController->ServerRepairBuildingActor(BuildingActorToRepair);
}

void AFortPlayerControllerAthenaPatch::ServerRemoveInventoryItemHook(AFortPlayerControllerAthena* PlayerController, FGuid ItemGuid, int Count, bool bForceRemoveFromQuickBars, bool bForceRemoval)
{
	PlayerController->ServerRemoveInventoryItem(ItemGuid, Count, bForceRemoval, bForceRemoval);
}

void AFortPlayerControllerAthenaPatch::ServerAttemptInventoryDropHook(AFortPlayerControllerAthena* PlayerController, FGuid ItemGuid, int Count)
{
	PlayerController->ServerAttemptInventoryDrop(ItemGuid, Count);
}

void AFortPlayerControllerAthenaPatch::ServerSuicideHook(AFortPlayerControllerAthena* PlayerController)
{
	PlayerController->ServerSuicide();
}

void AFortPlayerControllerAthenaPatch::ServerReturnToMainMenuHook(AFortPlayerControllerAthena* PlayerController)
{
	PlayerController->ServerReturnToMainMenu();
}

void AFortPlayerControllerAthenaPatch::ServerPlayEmoteItemHook(AFortPlayerControllerAthena* PlayerController, UFortMontageItemDefinitionBase* EmoteAsset)
{
	PlayerController->ServerPlayEmoteItem(EmoteAsset);
}

void AFortPlayerControllerAthenaPatch::ServerChangeNameHook(AFortPlayerControllerAthena* PlayerController, FString S)
{
	PlayerController->ServerChangeName(S);
}

void AFortPlayerControllerAthenaPatch::ServerAttemptAircraftJumpHook(AFortPlayerControllerAthena* PlayerController, FRotator ClientRotation)
{
	if (PlayerController->ServerAttemptAircraftJump(ClientRotation))
		_ServerAttemptAircraftJump(PlayerController, ClientRotation);
}

void AFortPlayerControllerAthenaPatch::ClientOnPawnDiedHook(AFortPlayerControllerAthena* PlayerController, FFortPlayerDeathReport& DeathReport)
{
	_ClientOnPawnDied(PlayerController, DeathReport);

	PlayerController->ClientOnPawnDied(DeathReport);
}

void AFortPlayerControllerAthenaPatch::EnterAircraftHook(AFortPlayerControllerAthena* PlayerController, AFortAthenaAircraft* Aircraft)
{
	PlayerController->EnterAircraft(Aircraft);

	_EnterAircraft(PlayerController, Aircraft);
}

void AFortPlayerControllerAthenaPatch::ExitAircraftHook(AFortPlayerControllerAthena* PlayerController)
{
	PlayerController->ExitAircraft();

	_ExitAircraft(PlayerController);
}

void AFortPlayerControllerAthenaPatch::GetPlayerViewpointHook(AFortPlayerControllerAthena* PlayerController, FVector& OutLocation, FRotator& OutRotation)
{
	if (!PlayerController->GetPlayerViewpoint(OutLocation, OutRotation))
		_GetPlayerViewpoint(PlayerController, OutLocation, OutRotation);
}

void AFortPlayerControllerAthenaPatch::execServerAttemptInteractHook(AFortPlayerControllerAthena* PlayerController, FFrame& Stack)
{
	PlayerController->ServerAttemptInteract(*(AActor**)Stack.Locals);

	_execServerAttemptInteract(PlayerController, Stack);
}

void AFortPlayerControllerAthenaPatch::execServerCreateBuildingActorHook(AFortPlayerControllerAthena* PlayerController, FFrame& Stack)
{
	struct FBuildingClassData
	{
		UClass* BuildingClass;
		int PreviousBuildingLevel;
		int UpgradeLevel;
	};

	struct FCreateBuildingActorData
	{
		uint32_t BuildingClassHandle;
		FVector BuildLoc;
		FRotator BuildRot;
		bool bMirrored;
		unsigned char UnknownData00[0x3];
		float SyncKey;
		unsigned char UnknownData01[0x4];
		FBuildingClassData BuildingClassData;
	};

	struct ParamsContainer
	{
		FBuildingClassData BuildingClassData;
		FVector BuildLoc;
		FRotator BuildRot;
		bool bMirrored;
		float SyncKey;
	};

	FVector Location;
	FRotator Rotation;
	bool bMirrored;
	UClass* BuildingClass = NULL;

	if (Stack.Node->PropertiesSize == sizeof(FCreateBuildingActorData))
	{
		auto Params = (FCreateBuildingActorData*)Stack.Locals;

		Location = Params->BuildLoc;
		Rotation = Params->BuildRot;
		bMirrored = Params->bMirrored;
		BuildingClass = PlayerController->BroadcastRemoteClientInfo->RemoteBuildableClass;
	}
	else
	{
		auto Params = (ParamsContainer*)Stack.Locals;

		Location = Params->BuildLoc;
		Rotation = Params->BuildRot;
		bMirrored = Params->bMirrored;
		BuildingClass = Params->BuildingClassData.BuildingClass;
	}

	PlayerController->ServerCreateBuildingActor(Location, Rotation, bMirrored, BuildingClass);
}

void AFortPlayerControllerAthenaPatch::execEndGhostModeHook(AFortPlayerControllerAthena* PlayerController, FFrame& Stack)
{
	PlayerController->EndGhostMode();

	_execEndGhostMode(PlayerController, Stack);
}

void AFortPlayerControllerAthenaPatch::execServerGiveCreativeItemHook(AFortPlayerControllerAthena* PlayerController, FFrame& Stack)
{
	PlayerController->ServerGiveCreativeItem((FFortItemEntry*)Stack.Locals);
}

bool AFortPlayerControllerAthenaPatch::IsTeamDeadHook(AFortPlayerControllerAthena* PlayerController)
{
	return PlayerController->IsTeamDead();
}

AActor* AFortPlayerControllerAthenaPatch::SpawnToyInstanceHook(AFortPlayerControllerAthena* PlayerController, FFrame& Stack, AActor** ReturnValue)
{
	UClass* ToyClass = NULL;
	FTransform SpawnPosition;

	Stack.StepCompiledIn(&ToyClass);
	Stack.StepCompiledIn(&SpawnPosition);

	*ReturnValue = PlayerController->SpawnToyInstance(ToyClass, SpawnPosition);
	return *ReturnValue;
}

TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* AFortPlayerControllerAthenaPatch::FindTeamMembersHook(AFortPlayerControllerAthena* PlayerController)
{
	if (auto TeamMembers = PlayerController->FindTeamMembers())
		return TeamMembers;

	return _FindTeamMembersPC(PlayerController);
}

void AFortPlayerControllerAthenaPatch::ServerTeleportToPlaygroundLobbyIslandHook(AFortPlayerControllerAthena* PlayerController)
{
	PlayerController->ServerTeleportToPlaygroundLobbyIsland();
}

void AFortPlayerControllerAthenaPatch::ServerTeleportToPlaygroundIslandDockHook(AFortPlayerControllerAthena* PlayerController, FFrame& Stack)
{
	PlayerController->ServerTeleportToPlaygroundIslandDock();
}

void AFortPlayerControllerAthenaPatch::execServerStartMinigameHook(AFortPlayerControllerAthena* PlayerController, FFrame& Stack)
{
	PlayerController->ServerStartMinigame();

	_execServerStartMinigame(PlayerController, Stack);
}

void AFortPlayerControllerAthenaPatch::ServerCreativeSetFlightSpeedIndexHook(AFortPlayerControllerAthena* PlayerController, int32 Index)
{
	PlayerController->ServerCreativeSetFlightSpeedIndex(Index);
}

void AFortPlayerControllerAthenaPatch::ServerCreativeSetFlightSprintHook(AFortPlayerControllerAthena* PlayerController, bool bSprint)
{
	PlayerController->ServerCreativeSetFlightSprint(bSprint);
}

void AFortPlayerControllerAthenaPatch::ServerUpdateActorOptionsHook(AFortPlayerControllerAthena* PlayerController, AActor* OptionsTarget, TArray<FString>& OptionsKeys, TArray<FString>& OptionsValues)
{
	PlayerController->ServerUpdateActorOptions(OptionsTarget, OptionsKeys, OptionsValues);

	_ServerUpdateActorOptions(PlayerController, OptionsTarget, OptionsKeys, OptionsValues);
}

bool AFortPlayerControllerAthenaPatch::ServerLoadPlotForPortal_ValidateHook(AFortPlayerControllerAthena* PlayerController, AFortAthenaCreativePortal* Portal, FString& PlotItemId)
{
	return PlayerController->ServerLoadPlotForPortal_Validate(Portal, PlotItemId);
}

void AFortPlayerControllerAthenaPatch::ServerLoadPlotForPortalHook(AFortPlayerControllerAthena* PlayerController, AFortAthenaCreativePortal* Portal, FString& PlotItemId)
{
	PlayerController->ServerLoadPlotForPortal(Portal, PlotItemId);
}

void AFortPlayerControllerAthenaPatch::Init()
{
	_EnterAircraft = UKismetMemoryLibrary::Get<decltype(_EnterAircraft)>(L"AFortPlayerControllerAthena::EnterAircraft");
	_ExitAircraft = UKismetMemoryLibrary::Get<decltype(_ExitAircraft)>(L"AFortPlayerControllerAthena::ExitAircraft");
	_GetPlayerViewpoint = UKismetMemoryLibrary::Get<decltype(_GetPlayerViewpoint)>(L"AFortPlayerController::GetPlayerViewpoint");
	_ClientOnPawnDied = UKismetMemoryLibrary::Get<decltype(_ClientOnPawnDied)>(L"AFortPlayerControllerZone::ClientOnPawnDied");
	_IsTeamDead = UKismetMemoryLibrary::Get<decltype(_IsTeamDead)>(L"AFortPlayerControllerAthena::IsTeamDead");
	_FindTeamMembersPC = UKismetMemoryLibrary::Get<decltype(_FindTeamMembersPC)>(L"AFortPlayerControllerAthena::FindTeamMembers");

	if (UFortControllerComponent_Aircraft::StaticClass() == NULL)
	{
		EnterAircraftDetour = new UDetour();
		EnterAircraftDetour->Init(_EnterAircraft, EnterAircraftHook);
		EnterAircraftDetour->Commit();

		ExitAircraftDetour = new UDetour();
		ExitAircraftDetour->Init(_ExitAircraft, ExitAircraftHook);
		ExitAircraftDetour->Commit();
	}

	GetPlayerViewpointDetour = new UDetour();
	GetPlayerViewpointDetour->Init(_GetPlayerViewpoint, GetPlayerViewpointHook);
	GetPlayerViewpointDetour->Commit();

	ClientOnPawnDiedDetour = new UDetour();
	ClientOnPawnDiedDetour->Init(_ClientOnPawnDied, ClientOnPawnDiedHook);
	ClientOnPawnDiedDetour->Commit();

	IsTeamDeadDetour = new UDetour();
	IsTeamDeadDetour->Init(_IsTeamDead, IsTeamDeadHook);
	IsTeamDeadDetour->Commit();

	FindTeamMembersPCDetour = new UDetour();
	FindTeamMembersPCDetour->Init(_FindTeamMembersPC, FindTeamMembersHook);
	FindTeamMembersPCDetour->Commit();

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerAcknowledgePossession", 
		EReflectedDetourType::VFSwap, 
		ServerAcknowledgePossessionHook,
		_ServerAcknowledgePossession,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerExecuteInventoryItem",
		EReflectedDetourType::VFSwap,
		ServerExecuteInventoryItemHook,
		_ServerExecuteInventoryItem,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerCreateBuildingActor",
		EReflectedDetourType::ExecSwap,
		execServerCreateBuildingActorHook,
		_execServerCreateBuildingActor,
		0
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerEditBuildingActor",
		EReflectedDetourType::VFSwap,
		ServerEditBuildingActorHook,
		_ServerEditBuildingActor,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerBeginEditingBuildingActor",
		EReflectedDetourType::VFSwap,
		ServerBeginEditingBuildingActorHook,
		_ServerBeginEditingBuildingActor,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerEndEditingBuildingActor",
		EReflectedDetourType::VFSwap,
		ServerEndEditingBuildingActorHook,
		_ServerEndEditingBuildingActor,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerRepairBuildingActor",
		EReflectedDetourType::VFSwap,
		ServerRepairBuildingActorHook,
		_ServerRepairBuildingActor,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerRemoveInventoryItem",
		EReflectedDetourType::VFSwap,
		ServerRemoveInventoryItemHook,
		_ServerRemoveInventoryItem,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerAttemptInventoryDrop",
		EReflectedDetourType::VFSwap,
		ServerAttemptInventoryDropHook,
		_ServerAttemptInventoryDrop,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerSuicide",
		EReflectedDetourType::VFSwap,
		ServerSuicideHook,
		_ServerSuicide,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerReturnToMainMenu",
		EReflectedDetourType::VFSwap,
		ServerReturnToMainMenuHook,
		_ServerReturnToMainMenu,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerAttemptInteract",
		EReflectedDetourType::ExecSwap,
		execServerAttemptInteractHook,
		_execServerAttemptInteract
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerPlayEmoteItem",
		EReflectedDetourType::VFSwap,
		ServerPlayEmoteItemHook,
		_ServerPlayEmoteItem,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerChangeName",
		EReflectedDetourType::VFSwap,
		ServerChangeNameHook,
		_ServerChangeName,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerAttemptAircraftJump",
		EReflectedDetourType::VFSwap,
		ServerAttemptAircraftJumpHook,
		_ServerAttemptAircraftJump,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"EndGhostMode",
		EReflectedDetourType::ExecSwap,
		execEndGhostModeHook,
		_execEndGhostMode
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerGiveCreativeItem",
		EReflectedDetourType::ExecSwap,
		execServerGiveCreativeItemHook,
		_execServerGiveCreativeItem,
		0
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerStartMinigame",
		EReflectedDetourType::ExecSwap,
		execServerStartMinigameHook,
		_execServerStartMinigame,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"SpawnToyInstance",
		EReflectedDetourType::ExecSwap,
		SpawnToyInstanceHook,
		_SpawnToyInstance
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerTeleportToPlaygroundLobbyIsland",
		EReflectedDetourType::VFSwap,
		ServerTeleportToPlaygroundLobbyIslandHook,
		_ServerTeleportToPlaygroundLobbyIsland,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerTeleportToPlaygroundIslandDock",
		EReflectedDetourType::VFSwap,
		ServerTeleportToPlaygroundIslandDockHook,
		_ServerTeleportToPlaygroundIslandDock,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerCreativeSetFlightSpeedIndex",
		EReflectedDetourType::VFSwap,
		ServerCreativeSetFlightSpeedIndexHook,
		_ServerCreativeSetFlightSpeedIndex,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerCreativeSetFlightSprint",
		EReflectedDetourType::VFSwap,
		ServerCreativeSetFlightSprintHook,
		_ServerCreativeSetFlightSprint,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerUpdateActorOptions",
		EReflectedDetourType::VFSwap,
		ServerUpdateActorOptionsHook,
		_ServerUpdateActorOptions,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerLoadPlotForPortal",
		EReflectedDetourType::VFSwap,
		ServerLoadPlotForPortal_ValidateHook,
		_ServerLoadPlotForPortal_Validate,
		0
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerControllerAthena>(
		L"ServerLoadPlotForPortal",
		EReflectedDetourType::VFSwap,
		ServerLoadPlotForPortalHook,
		_ServerLoadPlotForPortal,
		1
	);

	*UKismetMemoryLibrary::Get<uint8*>(L"AFortPlayerController::RemoveItem") = -1;
}