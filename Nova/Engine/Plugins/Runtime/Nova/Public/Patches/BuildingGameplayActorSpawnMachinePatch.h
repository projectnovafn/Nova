#pragma once
#include "Building/BuildingGameplayActorSpawnMachine.h"

class ABuildingGameplayActorSpawnMachinePatch
{
private:
	static void ResurrectPlayerHook(ABuildingGameplayActorSpawnMachine*);

public:

	static void Init();
};