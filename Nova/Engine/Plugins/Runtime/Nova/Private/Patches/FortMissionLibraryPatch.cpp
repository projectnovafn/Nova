#include "Patches/FortMissionLibraryPatch.h"
#include <KismetDetoursLibrary.h>

static void (*_TeleportPlayerPawn)(AFortPlayerPawn* PlayerPawn, const FVector& DestLocation, const FRotator& DestRotation, bool bIgnoreCollision, bool bIgnoreSupplementalKillVolumeSweep);


void UFortMissionLibraryPatch::TeleportPlayerPawnHook(AFortPlayerPawn* PlayerPawn, const FVector& DestLocation, const FRotator& DestRotation, bool bIgnoreCollision, bool bIgnoreSupplementalKillVolumeSweep)
{
	UFortMissionLibrary::TeleportPlayerPawn(PlayerPawn, DestLocation, DestRotation, bIgnoreCollision, bIgnoreSupplementalKillVolumeSweep);
}

void UFortMissionLibraryPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<UFortMissionLibrary>(
		L"TeleportPlayerPawn",
		EReflectedDetourType::ImplSwap,
		TeleportPlayerPawnHook,
		_TeleportPlayerPawn
	);
}
