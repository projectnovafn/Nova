#include "Patches/FortOctopusVehiclePatch.h"
#include "KismetDetoursLibrary.h"

void (*_ServerUpdateTowhook)(AFortOctopusVehicle*, FVector);
void (*_SetTowhookAttachState)(AFortOctopusVehicle*, UPrimitiveComponent*, FVector, FVector);
void (*_ServerSetTowhookAttachState)(AFortOctopusVehicle*, FNetTowhookAttachState);

void AFortOctopusVehiclePatch::ServerUpdateTowhookHook(AFortOctopusVehicle* Vehicle, FVector InNetTowhookAimDir)
{
	Vehicle->ServerUpdateTowhook(InNetTowhookAimDir);
}

void AFortOctopusVehiclePatch::ServerSetTowhookAttachStateHook(AFortOctopusVehicle* Vehicle, FNetTowhookAttachState InTowHookAttachState)
{
	Vehicle->ServerSetTowhookAttachState(InTowHookAttachState);
}

void AFortOctopusVehiclePatch::SetTowhookAttachStateHook(AFortOctopusVehicle* Vehicle, UPrimitiveComponent* Component, FVector WorldLocation, FVector WorldNormal)
{
	Vehicle->SetTowhookAttachState(Component, WorldLocation, WorldNormal);
}

void AFortOctopusVehiclePatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<AFortOctopusVehicle>(
		L"ServerUpdateTowhook",
		EReflectedDetourType::VFSwap,
		ServerUpdateTowhookHook,
		_ServerUpdateTowhook,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortOctopusVehicle>(
		L"ServerSetTowhookAttachState",
		EReflectedDetourType::VFSwap,
		ServerSetTowhookAttachStateHook,
		_ServerSetTowhookAttachState,
		1
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortOctopusVehicle>(
		L"SetTowhookAttachState",
		EReflectedDetourType::ImplSwap,
		SetTowhookAttachStateHook,
		_SetTowhookAttachState,
		0,
		{ 0x48, 0x03 }
	);
}