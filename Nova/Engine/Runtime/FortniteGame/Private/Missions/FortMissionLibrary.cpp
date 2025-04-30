#include "Missions/FortMissionLibrary.h"

#include "Pawns/FortPlayerPawnAthena.h"
#include "Athena/Player/FortPlayerControllerAthena.h"

void UFortMissionLibrary::TeleportPlayerPawn(AFortPlayerPawn* PlayerPawn, const FVector& DestLocation, const FRotator& DestRotation, bool bIgnoreCollision, bool bIgnoreSupplementalKillVolumeSweep)
{
	auto PlayerController = Cast<AFortPlayerControllerAthena>(PlayerPawn->Controller);

	if (auto CharacterMovement = PlayerPawn->CharacterMovement)
	{
		if (CharacterMovement->MovementMode == EMovementMode::Flying)
		{
			CharacterMovement->Velocity = FVector();

			PlayerController->ServerCreativeToggleFly();			
		}
	}

	PlayerPawn->TeleportTo(DestLocation, DestRotation);
}