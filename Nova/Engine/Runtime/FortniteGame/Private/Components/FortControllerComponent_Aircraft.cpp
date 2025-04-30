#include "Components/FortControllerComponent_Aircraft.h"
#include "Athena/Player/FortPlayerControllerAthena.h"

void UFortControllerComponent_Aircraft::ServerAttemptAircraftJump(FRotator ClientRotation)
{
	auto PlayerController = Cast<AFortPlayerControllerAthena>(GetOwner());

	if (!PlayerController)
		return;

	PlayerController->ServerAttemptAircraftJump(ClientRotation, true);
}

void UFortControllerComponent_Aircraft::EnterAircraft(AFortAthenaAircraft* Aircraft)
{
	auto PlayerController = Cast<AFortPlayerControllerAthena>(GetOwner());

	if (!PlayerController)
		return;

	PlayerController->EnterAircraft(Aircraft);
}

void UFortControllerComponent_Aircraft::ExitAircraft()
{
	auto PlayerController = Cast<AFortPlayerControllerAthena>(GetOwner());

	if (!PlayerController)
		return;

	PlayerController->ExitAircraft();
}