#pragma once
#include "Components/ActorComponent.h"
#include "ItemDefinitions/FortPlaysetItemDefinition.h"

class UPlaysetLevelStreamComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY(UPlaysetLevelStreamComponent, FortniteGame)
public:

	void SetPlayset(UFortPlaysetItemDefinition* NewPlayset);

	void LoadPlaysetAsync();
	void UnloadPlaysetAsync();
};