#pragma once
#include "FortWorkerType.h"
#include "FortHeroSpecialization.h"

class UFortHeroType : public UFortWorkerType
{
	GENERATED_UCLASS_BODY(UFortHeroType, FortniteGame)
public:

	UPROPERTY(TArray<TSoftObjectPtr<UFortHeroSpecialization>>, Specializations)
	TArray<TSoftObjectPtr<UFortHeroSpecialization>> Specializations;
};