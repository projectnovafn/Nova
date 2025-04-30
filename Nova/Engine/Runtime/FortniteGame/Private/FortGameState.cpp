#include "FortGameState.h"

#include "Player/FortPlayerState.h"

AFortPlayerController* AFortGameState::FindPlayerByProfile(UMcpProfileGroup* McpProfileGroup)
{
	for (auto& PlayerState : PlayerArray)
	{
		auto PlayerController = Cast<AFortPlayerController>(PlayerState->GetOwner());

		if (!PlayerController)
			continue;

		if (PlayerController->McpProfileGroup == McpProfileGroup)
		{
			return PlayerController;
		}
	}

	return NULL;
}

AFortPlayerState* AFortGameState::FindPlayerById(FUniqueNetIdRepl& UniqueId)
{
	auto NetDriver = GetWorld()->NetDriver;

	if (!NetDriver)
		return NULL;

	for (int i = 0; i < NetDriver->ClientConnections.Num(); i++)
	{
		auto Connection = NetDriver->ClientConnections[i];
		if (!Connection) continue;

		auto PlayerController = Connection->PlayerController;
		if (!PlayerController) continue;

		auto PlayerState = PlayerController->PlayerState;
		if (!PlayerState) continue;

		if (AFortPlayerState::AreUniqueIDsIdentical(*PlayerState->UniqueId, UniqueId))
			return Cast<AFortPlayerState>(PlayerState);
	}

	return NULL;
}