#pragma once
#include "CoreUObject.h"

class FGameplayTag
{
    GENERATED_USTRUCT_BODY(FGameplayTag, GameplayTags)

public:
    
    FGameplayTag() : TagName(NAME_None) { }
    FGameplayTag(FName InTagName) : TagName(InTagName) { }

    FName TagName;
};