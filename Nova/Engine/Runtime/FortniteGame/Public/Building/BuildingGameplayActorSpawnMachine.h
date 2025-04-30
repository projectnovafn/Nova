#pragma once
#include "Building/BuildingGameplayActor.h"

#include "FortPlayerStart.h"
#include "WeakReference.h"

class AFortPlayerControllerAthena;
class AFortPlayerPawn;

enum ESpawnMachineState : uint8
{
	Default = 0,
	WaitingForUse = 1,
	Active = 2,
	Complete = 3,
	OnCooldown = 4
};

class ABuildingGameplayActorSpawnMachine : public ABuildingGameplayActor
{
	GENERATED_UCLASS_BODY(ABuildingGameplayActorSpawnMachine, FortniteGame)
public:

	UPROPERTY(AFortPlayerStart*, ResurrectLocation)
	AFortPlayerStart* ResurrectLocation;

	UPROPERTY(TArray<FUniqueNetIdRepl>, PlayerIdsForResurrection)
	TArray<FUniqueNetIdRepl> PlayerIdsForResurrection;

	UPROPERTY(TWeakObjectPtr<AFortPlayerControllerAthena>, InstigatorPC)
	TWeakObjectPtr<AFortPlayerControllerAthena> InstigatorPC;

	UPROPERTY(uint8, SquadId)
	uint8 SquadId;

	UPROPERTY(uint8, ActiveTeam)
	uint8 ActiveTeam;

	UPROPERTY(ESpawnMachineState, SpawnMachineState)
	ESpawnMachineState SpawnMachineState;

	void OnRep_SpawnMachineState();

	void Setup();

	void ResurrectPlayer();
	void OnPlayerPawnResurrected(AFortPlayerPawn* PlayerPawn);
	void OnResurrectionCompleted();
};