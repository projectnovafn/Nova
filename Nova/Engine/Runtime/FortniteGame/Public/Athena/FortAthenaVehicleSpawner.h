#pragma once
#include "GameFramework/Actor.h"

class AFortAthenaVehicleSpawner : public AActor
{
	GENERATED_UCLASS_BODY(AFortAthenaVehicleSpawner, FortniteGame)
public:

	void SpawnVehicle();
	UClass* GetVehicleClass();
};