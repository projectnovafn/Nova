#pragma once
#include "Components/FortControllerComponent_Aircraft.h"

class UFortControllerComponent_AircraftPatch
{
public:

	static void ServerAttemptAircraftJumpHook(UFortControllerComponent_Aircraft*, FRotator);
	static void EnterAircraftHook(UFortControllerComponent_Aircraft*, AFortAthenaAircraft*);
	static void ExitAircraftHook(UFortControllerComponent_Aircraft*);

	static void Init();
};