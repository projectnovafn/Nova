#include "Weapons/FortWeaponRangedMountedCannon.h"
#include "Pawns/FortPlayerPawnAthena.h"
#include "Vehicles/FortAthenaSKPushCannon.h"

void AFortWeaponRangedMountedCannon::ServerFireActorInCannon(FVector LaunchDir)
{
	auto Pawn = Cast<AFortPlayerPawnAthena>(GetOwner());

	UE_LOG(LogFort, Log, TEXT("ServerFireActorInCannon: %s"), *Pawn->GetName());

	if (!Pawn)
		return;

	auto Cannon = Cast<AFortAthenaSKPushCannon>(Pawn->GetVehicle());

	UE_LOG(LogFort, Log, TEXT("ServerFireActorInCannon: %s"), *Cannon->GetName());

	if (!Cannon)
		return;

	if (auto TargetPawn = Cannon->GetPawnAtSeat(1))
	{
		Cannon->OnPreLaunchPawn(TargetPawn, LaunchDir);

		TargetPawn->ServerOnExitVehicle(ETryExitVehicleBehavior::ForceAlways);

		Cannon->OnLaunchPawn(TargetPawn, LaunchDir);
		Cannon->MultiCastPushCannonLaunchedPlayer();
	}
}