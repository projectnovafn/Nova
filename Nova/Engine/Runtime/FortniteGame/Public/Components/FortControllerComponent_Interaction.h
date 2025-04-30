#pragma once
#include "Components/FortControllerComponent.h"
#include "GameFramework/Actor.h"

class UFortControllerComponent_Interaction : public UFortControllerComponent
{
	GENERATED_UCLASS_BODY(UFortControllerComponent_Interaction, FortniteGame)
public:

	void ServerAttemptInteract(AActor* ReceivingActor);
};