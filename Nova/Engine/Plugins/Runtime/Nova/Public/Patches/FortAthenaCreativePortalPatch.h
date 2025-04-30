#pragma once
#include "Athena/Creative/FortAthenaCreativePortal.h"

class AFortAthenaCreativePortalPatch
{
	static void TeleportPlayerToLinkedVolumeHook(AFortAthenaCreativePortal*, AFortPlayerPawnAthena*, bool);

public:

	static void Init();
};