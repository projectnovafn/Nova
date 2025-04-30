#include "Athena/FortAthenaVehicleSpawner.h"
#include "Engine/World.h"
#include "Vehicles/FortAthenaVehicle.h"
#include "Vehicles/FortOctopusVehicle.h"
#include "Vehicles/FortAthenaFerretVehicle.h"

void AFortAthenaVehicleSpawner::SpawnVehicle()
{
	auto VehicleClass = GetVehicleClass();
	
	if (!VehicleClass || VehicleClass->IsChildOf<AFortOctopusVehicle>())
		return;

	GetWorld()->SpawnActor<AFortAthenaVehicle>(GetActorLocation(), GetActorRotation(), VehicleClass);
}

UClass* AFortAthenaVehicleSpawner::GetVehicleClass()
{
	UClass* ReturnValue;
	this->ProcessEvent(L"GetVehicleClass", &ReturnValue);

	return ReturnValue;
}