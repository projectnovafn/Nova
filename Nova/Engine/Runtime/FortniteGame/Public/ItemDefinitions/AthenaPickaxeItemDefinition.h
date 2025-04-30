#pragma once
#include "AthenaCosmeticItemDefinition.h"

class UAthenaPickaxeItemDefinition : public UAthenaCosmeticItemDefinition
{
	GENERATED_UCLASS_BODY(UAthenaPickaxeItemDefinition, FortniteGame)
public:

	UPROPERTY(UFortItemDefinition*, WeaponDefinition)
	UFortItemDefinition* WeaponDefinition;
};