#include "Patches/FortMinigamePatch.h"
#include "Player/FortPlayerState.h"

#include "KismetDetoursLibrary.h"

static void (*_SetState)(AFortMinigame*, EFortMinigameState);
static void (*_NativeExitedVolumeEvent)(AFortMinigame*, AFortPlayerState*);
static void (*_NativeEnteredVolumeEvent)(AFortMinigame*, AFortPlayerState*);
static void (*_execClearPlayerInventory)(AFortMinigame*, FFrame&);

UDetour* SetStateDetour = NULL;

void AFortMinigamePatch::SetStateHook(AFortMinigame* Minigame, EFortMinigameState State)
{
	_SetState(Minigame, State);

	Minigame->SetState(State);
}

void AFortMinigamePatch::NativeExitedVolumeEventHook(AFortMinigame* Minigame, AFortPlayerState* LeftPlayerState)
{
	Minigame->NativeExitedVolumeEvent(LeftPlayerState);

	_NativeExitedVolumeEvent(Minigame, LeftPlayerState);
}

void AFortMinigamePatch::NativeEnteredVolumeEventHook(AFortMinigame* Minigame, AFortPlayerState* PlayerState)
{
	Minigame->NativeEnteredVolumeEvent(PlayerState);

	_NativeEnteredVolumeEvent(Minigame, PlayerState);
}

void AFortMinigamePatch::execClearPlayerInventoryHook(AFortMinigame* Minigame, FFrame& Stack)
{
	AFortPlayerControllerAthena* Controller;

	Stack.StepCompiledIn(&Controller);

	Minigame->ClearPlayerInventory(Controller);

	//_execClearPlayerInventory(Minigame, Stack);
}

void AFortMinigamePatch::Init()
{
	_SetState = UKismetMemoryLibrary::Get<decltype(_SetState)>(L"AFortMinigame::SetState");

	SetStateDetour = new UDetour();
	SetStateDetour->Init(_SetState, SetStateHook);
	SetStateDetour->Commit();

	UKismetDetoursLibrary::AddReflectedDetour<AFortMinigame>(
		L"NativeExitedVolumeEvent",
		EReflectedDetourType::ImplDetour,
		NativeExitedVolumeEventHook,
		_NativeExitedVolumeEvent,
		0,
		{ 0x48, 0x03 }
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortMinigame>(
		L"NativeEnteredVolumeEvent",
		EReflectedDetourType::ImplDetour,
		NativeEnteredVolumeEventHook,
		_NativeEnteredVolumeEvent,
		0,
		{ 0x48, 0x03 }
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortMinigame>(
		L"ClearPlayerInventory",
		EReflectedDetourType::ExecSwap,
		execClearPlayerInventoryHook,
		_execClearPlayerInventory
	);
}