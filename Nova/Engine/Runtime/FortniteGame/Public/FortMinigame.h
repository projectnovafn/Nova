#pragma once
#include "GameFramework/Info.h"
#include "Athena/Player/FortPlayerControllerAthena.h"

class AFortPlayerState;

enum class EFortMinigameState : uint8
{
	PreGame = 0,
	Setup = 1,
	Transitioning = 2,
	WaitingForCameras = 3,
	Warmup = 4,
	InProgress = 5,
	PostGameTimeDilation = 6,
	PostRoundEnd = 7,
	PostGameEnd = 8,
	PostGameAbandon = 9,
	PostGameReset = 10
};

class AFortMinigame : public AInfo
{
	GENERATED_UCLASS_BODY(AFortMinigame, FortniteGame)
public:

	UPROPERTY_NOTIFY(EFortMinigameState, CurrentState)
	EFortMinigameState CurrentState;

	void SetState(EFortMinigameState State);

	void NativeExitedVolumeEvent(AFortPlayerState* LeftPlayerState);
	void NativeEnteredVolumeEvent(AFortPlayerState* PlayerState);
	void ClearPlayerInventory(AFortPlayerControllerAthena* PlayerController);
};