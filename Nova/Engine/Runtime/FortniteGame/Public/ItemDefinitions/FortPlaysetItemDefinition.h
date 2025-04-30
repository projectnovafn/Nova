#pragma once
#include "FortItem.h"
#include "LevelSaveRecord.h"

class UFortPlaysetItemDefinition : public UFortItemDefinition
{
	GENERATED_UCLASS_BODY(UFortPlaysetItemDefinition, FortniteGame)
public:

	UPROPERTY(ULevelSaveRecord*, LevelSaveRecord)
	ULevelSaveRecord* LevelSaveRecord;

	static void Setup();
};