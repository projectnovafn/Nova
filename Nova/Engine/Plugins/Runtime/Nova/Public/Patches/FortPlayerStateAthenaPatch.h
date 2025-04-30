#pragma once
#include "Athena/Player/FortPlayerStateAthena.h"

class AFortPlayerStateAthenaPatch
{
private:

	static void ServerSetInAircraftHook(AFortPlayerStateAthena*, bool);

public:

	static void Init();
};