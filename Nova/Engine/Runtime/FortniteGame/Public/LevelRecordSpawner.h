#pragma once
#include "Core.h"
#include "FortVolume.h"

class ULevelRecordSpawner : public UObject
{
	GENERATED_UCLASS_BODY(ULevelRecordSpawner, FortniteGame)
public:

	UPROPERTY(AFortVolume*, VolumeCurrentlySpawningWithin)
	AFortVolume* VolumeCurrentlySpawningWithin;
};