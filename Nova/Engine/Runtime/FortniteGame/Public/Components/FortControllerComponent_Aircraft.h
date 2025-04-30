#pragma once
#include "Components/FortControllerComponent.h"
#include "GameFramework/Actor.h"

#include "Athena/FortAthenaAircraft.h"

class UFortControllerComponent_Aircraft : public UFortControllerComponent
{
	GENERATED_UCLASS_BODY(UFortControllerComponent_Aircraft, FortniteGame)
public:

	void ServerAttemptAircraftJump(FRotator ClientRotation);
	void EnterAircraft(AFortAthenaAircraft* Aircraft);
	void ExitAircraft();
};