#pragma once

#include "BuildingSMActor.h"

class ABuildingTrap : public ABuildingSMActor
{
	GENERATED_UCLASS_BODY(ABuildingTrap, FortniteGame)

public:

	UPROPERTY(ABuildingSMActor*, AttachedTo)
	ABuildingSMActor* AttachedTo;

	UPROPERTY(UFortItemDefinition*, TrapData)
	UFortItemDefinition* TrapData;

public:

	void OnRep_AttachedTo() { this->ProcessEvent(L"OnRep_AttachedTo"); }
};