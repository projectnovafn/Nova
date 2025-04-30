#include "Patches/FortMinigameSettingsBuildingPatch.h"

#include "KismetDetoursLibrary.h"

static void (*_BeginPlay)(AFortMinigameSettingsBuilding*);

UDetour* MinigameBeginPlayDetour = NULL;

void AFortMinigameSettingsBuildingPatch::BeginPlayHook(AFortMinigameSettingsBuilding* SettingsBuilding)
{
	_BeginPlay(SettingsBuilding);

	SettingsBuilding->BeginPlay();
}

void AFortMinigameSettingsBuildingPatch::Init()
{
	_BeginPlay = UKismetMemoryLibrary::Get<decltype(_BeginPlay)>(L"AFortMinigameSettingsBuilding::BeginPlay");

	MinigameBeginPlayDetour = new UDetour();
	MinigameBeginPlayDetour->Init(_BeginPlay, BeginPlayHook);
	MinigameBeginPlayDetour->Commit();
}