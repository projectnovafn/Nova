#include "FortVolumeManager.h"
#include "FortVolume.h"

void AFortVolumeManager::ApplyLevelSaveToWorld(UObject* SaveToSpawnFrom, bool bComplexShouldSpawn)
{
	struct
	{
		UObject* SaveToSpawnFrom;
		bool bComplexShouldSpawn;
	} params(SaveToSpawnFrom, bComplexShouldSpawn);

	this->ProcessEvent(L"ApplyLevelSaveToWorld", &params);
}

AFortVolume* AFortVolumeManager::GetVolumeForActor(AActor* Actor)
{
	struct
	{
		AActor* Actor;
		AFortVolume* ReturnValue;
	} params(Actor);

	this->ProcessEvent(L"GetVolumeForActor", &params);

	return params.ReturnValue;
}