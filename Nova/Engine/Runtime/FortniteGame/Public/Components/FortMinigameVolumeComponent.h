#pragma once
#include "Components/ActorComponent.h"

class UFortMinigameVolumeComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY(UFortMinigameVolumeComponent, FortniteGame)
public:

	UPROPERTY(AActor*, CurrentMinigameSettingsMachine)
	AActor* CurrentMinigameSettingsMachine;
};