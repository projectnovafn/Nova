#pragma once
#include "GameFramework/PlayerStart.h"
#include "GameplayTagContainer.h"

class AFortPlayerStartCreative : public APlayerStart
{
	GENERATED_UCLASS_BODY(AFortPlayerStartCreative, FortniteGame)
public:

	UPROPERTY_BOOL(bUseAsIslandStart)
	bool bUseAsIslandStart;

	UPROPERTY_BOOL(bIsEnabled)
	bool bIsEnabled;

	UPROPERTY(FGameplayTagContainer, PlayerStartTags)
	FGameplayTagContainer PlayerStartTags;
};