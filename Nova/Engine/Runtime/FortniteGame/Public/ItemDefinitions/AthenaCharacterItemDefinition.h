#pragma once
#include "AthenaCosmeticItemDefinition.h"
#include "AthenaBackpackItemDefinition.h"
#include "FortHeroType.h"

class UAthenaCharacterItemDefinition : public UAthenaCosmeticItemDefinition
{
	GENERATED_UCLASS_BODY(UAthenaCharacterItemDefinition, FortniteGame)
public:

	UPROPERTY(UFortHeroType*, HeroDefinition)
	UFortHeroType* HeroDefinition;

	UPROPERTY(UAthenaBackpackItemDefinition*, DefaultBackpack)
	UAthenaBackpackItemDefinition* DefaultBackpack;
};