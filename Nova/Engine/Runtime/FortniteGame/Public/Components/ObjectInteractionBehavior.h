#pragma once
#include "Components/ActorComponent.h"

class UObjectInteractionBehavior : public UActorComponent
{
	GENERATED_UCLASS_BODY(UObjectInteractionBehavior, FortniteGame)
public:

	void StartCreativeInteractionOnServer();
};