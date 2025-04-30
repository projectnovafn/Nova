#include "Patches/FortPlayerStateAthenaPatch.h"
#include "KismetDetoursLibrary.h"

static void (*_ServerSetInAircraft)(AFortPlayerStateAthena*, bool);

void AFortPlayerStateAthenaPatch::ServerSetInAircraftHook(AFortPlayerStateAthena* PlayerState, bool bNewInAircraft)
{
	PlayerState->ServerSetInAircraft(bNewInAircraft);

	_ServerSetInAircraft(PlayerState, bNewInAircraft);
}

void AFortPlayerStateAthenaPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<AFortPlayerStateAthena>(
		L"ServerSetInAircraft",
		EReflectedDetourType::VFSwap,
		ServerSetInAircraftHook,
		_ServerSetInAircraft,
		1
	);
}