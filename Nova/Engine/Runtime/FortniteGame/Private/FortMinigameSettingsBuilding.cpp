#include "FortMinigameSettingsBuilding.h"

#include "Athena/FortGameStateAthena.h"

void AFortMinigameSettingsBuilding::BeginPlay()
{
	UE_LOG(LogFort, Log, TEXT("AFortMinigameSettingsBuilding::BeginPlay"));

	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (!GameState)
		return;

	SettingsVolume = GameState->VolumeManager->GetVolumeForActor(this);
}