#pragma once
#include "Vehicles/FortOctopusVehicle.h"

class AFortOctopusVehiclePatch
{
private:
	static void ServerUpdateTowhookHook(AFortOctopusVehicle*, FVector);
	static void ServerSetTowhookAttachStateHook(AFortOctopusVehicle*, FNetTowhookAttachState);
	static void SetTowhookAttachStateHook(AFortOctopusVehicle*, UPrimitiveComponent*, FVector, FVector);

public:

	static void Init();
};