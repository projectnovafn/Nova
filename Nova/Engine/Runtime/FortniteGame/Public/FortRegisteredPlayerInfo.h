#pragma once

#include "CoreUObject.h"
#include "GameFramework/OnlineReplStructs.h"

class UFortRegisteredPlayerInfo : public UObject
{
	GENERATED_UCLASS_BODY(UFortRegisteredPlayerInfo, FortniteGame)
public:

	UPROPERTY_PTR(FUniqueNetIdRepl, PlayerID)
	FUniqueNetIdRepl* PlayerID;
};