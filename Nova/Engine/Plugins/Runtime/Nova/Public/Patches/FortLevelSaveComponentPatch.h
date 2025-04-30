#pragma once
#include "Components/FortLevelSaveComponent.h"

class UFortLevelSaveComponentPatch
{
public:
	static UObject* GetSidecarHook(UFortLevelSaveComponent*);

	static void Init();
};