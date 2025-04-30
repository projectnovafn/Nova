#pragma once
#include "FortEngine.h"

class UFortEnginePatches
{
	static void InitHook(UFortEngine*, void*);

	static float GetMaxTickRateHook(UFortEngine*, float, bool);

public:
	static void Init();
};