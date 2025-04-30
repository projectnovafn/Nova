#include "FortTeamInfo.h"

#include "Athena/Player/FortPlayerControllerAthena.h"
#include "Athena/Player/FortPlayerStateAthena.h"

bool AFortTeamInfo::IsAlive()
{
	for (auto& TeamMember : TeamMembers)
	{
		auto PlayerController = Cast<AFortPlayerControllerAthena>(TeamMember);

		if (!PlayerController)
			continue;

		if (PlayerController->bMarkedAlive && (!PlayerController->MyFortPawn || !PlayerController->MyFortPawn->bIsDBNO))
			return true;
	}

	return false;
}