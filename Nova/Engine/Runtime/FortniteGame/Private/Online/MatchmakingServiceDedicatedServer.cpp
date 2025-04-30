#include "Online/MatchmakingServiceDedicatedServer.h"

void FMatchmakingServiceDedicatedServer::HandleWebSocketMessage(FString& Message)
{
	UE_LOG(LogFort, Log, TEXT("HandleWebSocketMessage: %s"), *Message);
}

void FMatchmakingServiceDedicatedServer::HandleWebSocketError(void*)
{
	FPlatformMisc::RequestExit(true);
}