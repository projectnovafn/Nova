#pragma once

#include "FortMontageItemDefinitionBase.h"
#include "UObject/ObjectMacros.h"

class UAthenaDanceItemDefinition : public UFortMontageItemDefinitionBase
{
	GENERATED_UCLASS_BODY(UAthenaDanceItemDefinition, FortniteGame)

public:

	UPROPERTY_BOOL(bMovingEmote)
	bool bMovingEmote;

	UPROPERTY_BOOL(bMoveForwardOnly)
	bool bMoveForwardOnly;

	UPROPERTY(float, WalkForwardSpeed)
	float WalkForwardSpeed;

};