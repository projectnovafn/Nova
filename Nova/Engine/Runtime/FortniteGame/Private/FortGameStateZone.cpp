#include "FortGameStateZone.h"

bool AFortGameStateZone::IsRespawningAllowed(AFortPlayerState* PlayerState)
{
	struct
	{
		AFortPlayerState* PlayerState;
		bool ReturnValue;
	} params(PlayerState);

	this->ProcessEvent(L"IsRespawningAllowed", &params);

	return params.ReturnValue;
}