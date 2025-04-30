#include "Patches/FortAthenaVehicleSpawnerPatch.h"
#include "KismetDetoursLibrary.h"

void (*_SpawnVehicle)(AFortAthenaVehicleSpawner*);

void AFortAthenaVehicleSpawnerPatch::SpawnVehicleHook(AFortAthenaVehicleSpawner* Spawner)
{
	Spawner->SpawnVehicle();
}

void AFortAthenaVehicleSpawnerPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<AFortAthenaVehicleSpawner>(
		L"SpawnVehicle",
		EReflectedDetourType::VFSwap,
		SpawnVehicleHook,
		_SpawnVehicle
	);
}