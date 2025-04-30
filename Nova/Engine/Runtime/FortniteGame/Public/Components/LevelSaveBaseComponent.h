#pragma once
#include "Components/ActorComponent.h"

class ULevelSaveBaseComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY(ULevelSaveBaseComponent, FortniteGame)
public:

	void SaveOutLevelRecord();
	void StartPeriodicSaveTimer();
	void StopPeriodicSaveTimer();
};