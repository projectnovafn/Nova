#include "GameplayTagContainer.h"

bool FGameplayTagContainer::Contains(FName Tag)
{
    for (auto& GameplayTag : GameplayTags)
    {
        if (GameplayTag.TagName == Tag)
            return true;
    }

    return false;
}