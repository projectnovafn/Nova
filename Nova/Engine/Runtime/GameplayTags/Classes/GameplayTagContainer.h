#pragma once
#include "CoreUObject.h"
#include "GameplayTag.h"

class FGameplayTagContainer
{
    GENERATED_USTRUCT_BODY(FGameplayTagContainer, GameplayTags)

public:
    TArray<FGameplayTag> GameplayTags;
    TArray<FGameplayTag> ParentTags;

    bool Contains(FName Tag);
};