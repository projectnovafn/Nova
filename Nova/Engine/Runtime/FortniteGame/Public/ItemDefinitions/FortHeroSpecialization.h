#pragma once
#include "Engine/DataAsset.h"
#include "CustomCharacterPart.h"

class UFortHeroSpecialization : public UPrimaryDataAsset
{
	GENERATED_UCLASS_BODY(UFortHeroSpecialization, FortniteGame)
public:

	UPROPERTY(TArray<TSoftObjectPtr<UCustomCharacterPart>>, CharacterParts)
	TArray<TSoftObjectPtr<UCustomCharacterPart>> CharacterParts;
};