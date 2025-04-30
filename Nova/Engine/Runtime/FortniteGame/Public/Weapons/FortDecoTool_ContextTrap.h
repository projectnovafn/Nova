#pragma once

#include "FortDecoTool.h"

class AFortDecoTool_ContextTrap : public AFortDecoTool
{
	GENERATED_UCLASS_BODY(AFortDecoTool_ContextTrap, FortniteGame)

public:

	UPROPERTY(UFortItemDefinition*, ContextTrapItemDefinition)
	UFortItemDefinition* ContextTrapItemDefinition;
};