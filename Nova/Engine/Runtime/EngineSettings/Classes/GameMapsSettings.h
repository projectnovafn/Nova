#pragma once
#include "CoreMinimal.h"
#include "CoreUObject.h"

class UGameMapsSettings : public UObject
{
	GENERATED_UCLASS_BODY(UGameMapsSettings, EngineSettings)

public:

	UPROPERTY(FSoftObjectPath, GameDefaultMap)
	FSoftObjectPath GameDefaultMap;
};