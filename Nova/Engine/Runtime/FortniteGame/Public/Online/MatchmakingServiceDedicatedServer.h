#pragma once
#include "Engine.h"

class FMatchmakingServiceDedicatedServer
{
public:

	void HandleWebSocketMessage(FString& Message);
	void HandleWebSocketError(void*);
};