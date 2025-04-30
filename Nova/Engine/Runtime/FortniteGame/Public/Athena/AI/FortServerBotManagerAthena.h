#pragma once
#include "CoreUObject.h"

class AFortGameModeAthena;
class AFortGameStateAthena;
class APawn;

class UFortServerBotManagerAthena : public UObject
{
	GENERATED_UCLASS_BODY(UFortServerBotManagerAthena, FortniteGame)
public:

	UPROPERTY(AFortGameModeAthena*, CachedGameMode)
	AFortGameModeAthena* CachedGameMode;

	UPROPERTY(AFortGameStateAthena*, CachedGameState)
	AFortGameStateAthena* CachedGameState;

	APawn* SpawnAI(FVector InSpawnLocation, FRotator InSpawnRotation, UObject* InBotData);
};