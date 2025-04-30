#pragma once
#include "Missions/FortMissionLibrary.h"
#include "Pawns/FortPlayerPawn.h"

class UFortMissionLibraryPatch
{
public:
	static void TeleportPlayerPawnHook(AFortPlayerPawn*, const FVector&, const FRotator&, bool, bool);

	static void Init();
};