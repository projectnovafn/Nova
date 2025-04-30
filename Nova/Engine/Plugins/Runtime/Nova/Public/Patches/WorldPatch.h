#pragma once
#include "Engine/World.h"
#include "KismetDetoursLibrary.h"

class UWorldPatch
{
private:
	static bool ListenHook(UWorld*);
	static void BeginPlayHook(UWorld*);

	static ENetMode GetNetModeHook(UWorld*);

	static void TickHook(UWorld*, ELevelTick, float);

public:
	static void Init();
};