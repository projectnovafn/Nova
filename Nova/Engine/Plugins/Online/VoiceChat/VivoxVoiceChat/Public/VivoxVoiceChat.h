#pragma once
#include "Core.h"
#include "GameFramework\OnlineReplStructs.h"

class FVivoxVoiceChat
{
public:
	FString GetJoinToken(FUniqueNetIdRepl& UniqueId, const FString& ChannelName);

	static FVivoxVoiceChat* Get();
};