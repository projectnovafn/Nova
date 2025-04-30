#include "Components/FortControllerComponent_Interaction.h"
#include "Athena/Player/FortPlayerControllerAthena.h"

void UFortControllerComponent_Interaction::ServerAttemptInteract(AActor* ReceivingActor)
{
	auto PlayerController = Cast<AFortPlayerController>(GetOwner());

	if (!PlayerController)
		return;

	PlayerController->ServerAttemptInteract(ReceivingActor);
}