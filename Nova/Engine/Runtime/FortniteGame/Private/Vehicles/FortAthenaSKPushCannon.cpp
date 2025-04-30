#include "Vehicles/FortAthenaSKPushCannon.h"

void AFortAthenaSKPushCannon::OnLaunchPawn(AFortPlayerPawn* Pawn, FVector LaunchDir)
{
	struct
	{
		AFortPlayerPawn* Pawn;
		FVector LaunchDir;
	} params(Pawn, LaunchDir);

	this->ProcessEvent(L"OnLaunchPawn", &params);
}

void AFortAthenaSKPushCannon::OnPreLaunchPawn(AFortPlayerPawn* Pawn, FVector LaunchDir)
{
	struct
	{
		AFortPlayerPawn* Pawn;
		FVector LaunchDir;
	} params(Pawn, LaunchDir);

	this->ProcessEvent(L"OnPreLaunchPawn", &params);
}

void AFortAthenaSKPushCannon::MultiCastPushCannonLaunchedPlayer()
{
	this->ProcessEvent(L"MultiCastPushCannonLaunchedPlayer");
}