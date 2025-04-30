#pragma once
#include "Info.h"
#include "GameFramework/OnlineReplStructs.h"

class APlayerState : public AInfo
{
	GENERATED_UCLASS_BODY(APlayerState, Engine)
public:

	UPROPERTY_PTR(FUniqueNetIdRepl, UniqueId)
	FUniqueNetIdRepl* UniqueId;

	UPROPERTY(int, PlayerID)
	int PlayerID;
};