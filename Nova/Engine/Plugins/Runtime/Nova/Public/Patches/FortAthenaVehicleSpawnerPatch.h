#pragma once
#include "Athena/FortAthenaVehicleSpawner.h"

class AFortAthenaVehicleSpawnerPatch
{
private:
	static void SpawnVehicleHook(AFortAthenaVehicleSpawner*);

public:
	static void Init();
};