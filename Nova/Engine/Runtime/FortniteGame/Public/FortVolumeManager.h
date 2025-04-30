#pragma once
#include "GameFramework/Info.h"

class AFortVolume;

class AFortVolumeManager : public AInfo
{
	GENERATED_UCLASS_BODY(AFortVolumeManager, FortniteGame)
public:

	void ApplyLevelSaveToWorld(UObject* SaveToSpawnFrom, bool bComplexShouldSpawn);

	AFortVolume* GetVolumeForActor(AActor* Actor);
};