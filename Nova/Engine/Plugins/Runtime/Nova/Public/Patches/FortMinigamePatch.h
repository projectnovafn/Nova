#pragma once
#include "FortMinigame.h"
#include "UObject/Stack.h"

class AFortPlayerState;

class AFortMinigamePatch
{
	static void SetStateHook(AFortMinigame*, EFortMinigameState);
	static void NativeExitedVolumeEventHook(AFortMinigame*, AFortPlayerState*);
	static void NativeEnteredVolumeEventHook(AFortMinigame*, AFortPlayerState*);

	static void execClearPlayerInventoryHook(AFortMinigame*, FFrame&);

public:

	static void Init();
};