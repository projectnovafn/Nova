#pragma once
#include "FortGamePvPBase.h"
#include "Athena/FortGameStateAthena.h"

#include "Athena/Player/FortPlayerStateAthena.h"
#include "Athena/Player/FortPlayerControllerAthena.h"
#include "Athena/AI/FortAthenaAIBotController.h"

#include "Athena/AI/FortServerBotManagerAthena.h"

#include "ItemDefinitions/FortWeaponItemDefinition.h"

typedef bool (*PlayerCanRestartType)(class AFortGameModeAthena*, APlayerController*);

class AFortGameModeAthena : public AFortGamePvPBase
{
	GENERATED_UCLASS_BODY(AFortGameModeAthena, FortniteGame)
public:
	
	UPROPERTY(TArray<FVector>, SafeZoneLocations)
	TArray<FVector> SafeZoneLocations;

	UPROPERTY(int, SafeZonePhase)
	int SafeZonePhase;

	UPROPERTY(TArray<AFortPlayerControllerAthena*>, AlivePlayers)
	TArray<AFortPlayerControllerAthena*> AlivePlayers;

	UPROPERTY(UFortServerBotManagerAthena*, ServerBotManager)
	UFortServerBotManagerAthena* ServerBotManager;

	void OnSafeZonePhaseChanged(int Phase);

	int GetLateSafeZoneIndex();

	FName RedirectLootTierGroup(const FName& TierGroup);

	void HandleMatchHasStarted();

	bool ReadyToStartMatch();
	bool ReadyToEndMatch();
	void InitGame();

	UClass* GetGameSessionClass();

	void AddToAlivePlayers(AFortPlayerControllerAthena* PlayerController);
	void RemoveFromAlivePlayers(AFortPlayerControllerAthena* PlayerController, APlayerState* KillerPlayerState, APawn* KillerPawn, UFortWeaponItemDefinition* KillerWeapon, uint8 DeathCause, bool bDBNO);

	void PostLogin(APlayerController* NewPlayer);
	
	void Tick();

	bool PlayerCanRestart(APlayerController* Player, PlayerCanRestartType Callback);

	APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot);
	void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot);

	AFortPlayerControllerAthena* PickSpectatingTarget(AFortPlayerControllerAthena* PlayerController);

	bool GenerateVoiceChatToken(AFortPlayerControllerZone* PlayerController, EVoiceChannelType ChannelType, FString const& ChannelName, FString& Token, FString& Error);
};