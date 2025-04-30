#pragma once
#include "FortAthenaSKPushVehicle.h"

class AFortAthenaSKPushCannon : public AFortAthenaSKPushVehicle
{
	GENERATED_UCLASS_BODY(AFortAthenaSKPushCannon, FortniteGame)
public:

	void OnLaunchPawn(AFortPlayerPawn* Pawn, FVector LaunchDir);
	void OnPreLaunchPawn(AFortPlayerPawn* Pawn, FVector LaunchDir);
	void MultiCastPushCannonLaunchedPlayer();
};