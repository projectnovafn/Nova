#include "Patches/FortAthenaCreativePortalPatch.h"

#include "KismetDetoursLibrary.h"

static void (*_TeleportPlayerToLinkedVolume)(AFortAthenaCreativePortal*, AFortPlayerPawnAthena*, bool);

void AFortAthenaCreativePortalPatch::TeleportPlayerToLinkedVolumeHook(AFortAthenaCreativePortal* Portal, AFortPlayerPawnAthena* Pawn, bool bUseSpawnTags)
{
	Portal->TeleportPlayerToLinkedVolume(Pawn, bUseSpawnTags);
}

void AFortAthenaCreativePortalPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<AFortAthenaCreativePortal>(
		L"TeleportPlayerToLinkedVolume",
		EReflectedDetourType::ImplSwap,
		TeleportPlayerToLinkedVolumeHook,
		_TeleportPlayerToLinkedVolume,
		0,
		{ 0x41, 0x0F }
	);
}