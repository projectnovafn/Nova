#pragma once

#include "Engine.h"

class AFortPlayerController;

class ABuildingActor : public AActor
{
	GENERATED_UCLASS_BODY(ABuildingActor, FortniteGame)

public:

	UPROPERTY_BOOL(bPlayerPlaced)
	bool bPlayerPlaced;

	UPROPERTY(uint8, TeamIndex)
	uint8 TeamIndex;

	UPROPERTY(uint8, Team)
	uint8 Team;

	UPROPERTY(int, CurrentBuildingLevel)
	int CurrentBuildingLevel;

public:

	float GetHealth();
	float GetMaxHealth();
	float GetHealthPercent();

	void OnRep_Team();

	void SetTeam(uint8 Team);
	uint8 GetTeam();

	void InitializeKismetSpawnedBuildingActor(ABuildingActor* BuildingOwner, AFortPlayerController* SpawningController, bool bUsePlayerBuildAnimations);
};