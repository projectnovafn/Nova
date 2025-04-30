#pragma once
#include "FortItem.h"
#include "FortPlaysetItemDefinition.h"

class UFortCreativeRealEstatePlotItemDefinition : public UFortItemDefinition
{
	GENERATED_UCLASS_BODY(UFortCreativeRealEstatePlotItemDefinition, FortniteGame)
public:

	UPROPERTY(TSoftObjectPtr<UFortPlaysetItemDefinition>, BasePlayset)
	TSoftObjectPtr<UFortPlaysetItemDefinition> BasePlayset;
};