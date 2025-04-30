#pragma once
#include "CoreUObject.h"
#include "OnlineIdentityMcp.h"

class FOnlineSubsystemMcp
{
public:

	bool ProcessRequestAsUser(void* McpService, void* UserId, void* HttpRequest, FString& Error);
	bool ProcessRequestAsClient(void* McpService, FString ClientId, void* HttpRequest, FString& Error);
};