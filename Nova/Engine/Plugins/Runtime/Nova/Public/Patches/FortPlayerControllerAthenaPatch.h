#pragma once

#include "UObject/Stack.h"

#include "Athena/Player/FortPlayerControllerAthena.h"
#include "Athena/Player/FortPlayerStateAthena.h"

#include "WeakReference.h"

class ABuildingSMActor;
class UFortMontageItemDefinitionBase;

class AFortPlayerControllerAthenaPatch
{
private:

	static void ServerAcknowledgePossessionHook(AFortPlayerControllerAthena*, APawn*);
	static void ServerExecuteInventoryItemHook(AFortPlayerControllerAthena*, FGuid);
	static void ServerEditBuildingActorHook(AFortPlayerControllerAthena*, ABuildingSMActor*, UClass*, int, bool);
	static void ServerBeginEditingBuildingActorHook(AFortPlayerControllerAthena*, ABuildingSMActor*);
	static void ServerEndEditingBuildingActorHook(AFortPlayerControllerAthena*, ABuildingSMActor*);
	static void ServerRepairBuildingActorHook(AFortPlayerControllerAthena*, ABuildingSMActor*);
	static void ServerRemoveInventoryItemHook(AFortPlayerControllerAthena*, FGuid, int, bool, bool);
	static void ServerAttemptInventoryDropHook(AFortPlayerControllerAthena*, FGuid, int);
	static void ServerSuicideHook(AFortPlayerControllerAthena*);
	static void ServerReturnToMainMenuHook(AFortPlayerControllerAthena*);
	static void ServerPlayEmoteItemHook(AFortPlayerControllerAthena*, UFortMontageItemDefinitionBase*);
	static void ServerChangeNameHook(AFortPlayerControllerAthena*, FString);
	static void ServerAttemptAircraftJumpHook(AFortPlayerControllerAthena*, FRotator);

	static void ClientOnPawnDiedHook(AFortPlayerControllerAthena*, FFortPlayerDeathReport&);

	static void EnterAircraftHook(AFortPlayerControllerAthena*, AFortAthenaAircraft*);
	static void ExitAircraftHook(AFortPlayerControllerAthena*);

	static void GetPlayerViewpointHook(AFortPlayerControllerAthena*, FVector&, FRotator&);

	static void execServerAttemptInteractHook(AFortPlayerControllerAthena*, FFrame&);
	static void execServerCreateBuildingActorHook(AFortPlayerControllerAthena*, FFrame&);
	static void execEndGhostModeHook(AFortPlayerControllerAthena*, FFrame&);
	static void execServerGiveCreativeItemHook(AFortPlayerControllerAthena*, FFrame&);

	static bool IsTeamDeadHook(AFortPlayerControllerAthena*);
	static AActor* SpawnToyInstanceHook(AFortPlayerControllerAthena*, FFrame&, AActor**);
	static TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* FindTeamMembersHook(AFortPlayerControllerAthena*);

	static void ServerTeleportToPlaygroundLobbyIslandHook(AFortPlayerControllerAthena*);
	static void ServerTeleportToPlaygroundIslandDockHook(AFortPlayerControllerAthena*, FFrame&);
	
	static void execServerStartMinigameHook(AFortPlayerControllerAthena*, FFrame&);

	static void ServerCreativeSetFlightSpeedIndexHook(AFortPlayerControllerAthena*, int32);
	static void ServerCreativeSetFlightSprintHook(AFortPlayerControllerAthena*, bool);

	static void ServerUpdateActorOptionsHook(AFortPlayerControllerAthena*, AActor*, TArray<FString>&, TArray<FString>&);

	static bool ServerLoadPlotForPortal_ValidateHook(AFortPlayerControllerAthena*, AFortAthenaCreativePortal*, FString&);
	static void ServerLoadPlotForPortalHook(AFortPlayerControllerAthena*, AFortAthenaCreativePortal*, FString&);

public:

	static void Init();
};