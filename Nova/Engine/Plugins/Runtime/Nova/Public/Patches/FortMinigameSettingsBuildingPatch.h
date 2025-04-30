#pragma once
#include "FortMinigameSettingsBuilding.h"

class AFortMinigameSettingsBuildingPatch
{
	static void BeginPlayHook(AFortMinigameSettingsBuilding* SettingsBuilding);

public:

	static void Init();
};