#pragma once
#include "CoreUObject.h"

class UFortGlobals : public UObject
{
	GENERATED_UCLASS_BODY(UFortGlobals, FortniteGame)
public:

	FString GetBRMapName();
	FString GetBRMapShortName();
};