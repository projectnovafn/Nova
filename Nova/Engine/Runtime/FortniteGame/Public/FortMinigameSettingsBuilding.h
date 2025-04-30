#pragma once
#include "GameFramework/Info.h"

#include "FortVolume.h"

class AFortMinigameSettingsBuilding : public AInfo
{
	GENERATED_UCLASS_BODY(AFortMinigameSettingsBuilding, FortniteGame)
public:

	UPROPERTY_NOTIFY(AFortVolume*, SettingsVolume)
	AFortVolume* SettingsVolume;

	void BeginPlay();
};