#pragma once
#include "AthenaCosmeticItemDefinition.h"
#include "CustomCharacterPart.h"

class UAthenaCharacterPartItemDefinition : public UAthenaCosmeticItemDefinition
{
	GENERATED_UCLASS_BODY(UAthenaCharacterPartItemDefinition, FortniteGame)
public:

	UPROPERTY(TArray<UCustomCharacterPart*>, CharacterParts)
	TArray<UCustomCharacterPart*> CharacterParts;
};