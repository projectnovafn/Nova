#include "ItemDefinitions/FortDecoItemDefinition.h"

UClass* UFortDecoItemDefinition::GetBlueprintClass()
{
    UClass* ReturnValue;
    this->ProcessEvent(L"GetBlueprintClass", &ReturnValue);

    return ReturnValue;
}
