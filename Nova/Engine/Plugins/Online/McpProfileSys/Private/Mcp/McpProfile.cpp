#include "Mcp/McpProfile.h"
#include "Engine.h"
#include "FortGameState.h"

#include "Athena/Player/FortPlayerControllerAthena.h"

void UMcpProfile::HandleProfileUpdate()
{
	auto GameState = Cast<AFortGameState>(GetWorld()->GameState);
	
	if (!GameState)
		return;

	auto PlayerController = GameState->FindPlayerByProfile(ProfileGroup);

	if (!PlayerController)
		return;

	if (ProfileId == TEXT("athena"))
	{
		PlayerController->HandleAthenaProfileUpdate();
	}
	else if (ProfileId == TEXT("creative"))
	{
		if (auto AthenaPlayerController = Cast<AFortPlayerControllerAthena>(PlayerController))
			AthenaPlayerController->HandleCreativeProfileUpdate();
	}
}