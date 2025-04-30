#pragma once
#include "Online/MatchmakingServiceDedicatedServer.h"

class FMatchmakingServiceDedicatedServerPatch
{
private:

	static void HandleWebSocketMessageHook(FMatchmakingServiceDedicatedServer*, FString&);
	static void HandleWebSocketErrorHook(FMatchmakingServiceDedicatedServer*, void*);

public:

	static void Init();
};