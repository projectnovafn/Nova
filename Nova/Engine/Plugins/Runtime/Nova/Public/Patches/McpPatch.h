#pragma once
#include "Engine/World.h"
#include "Mcp/McpProfile.h"
#include "OnlineIdentityMcp.h"
#include "OnlineSubsystemMcp.h"
#include "ServiceConfigMcp.h"

class McpPatch
{
private:

	static void SendRequestNowHook(UMcpProfileGroup*, void*, ENetMode);
	static void HandleProfileUpdateHook(UMcpProfile*, void*, void*);
	static bool AutoLoginHook(FOnlineIdentityMcp*, int32);
	static FServicePermissionsMcp* GetServicePermissionsByIdHook(FServiceConfigMcp*, FString&);
	static bool ProcessRequestAsUserHook(FOnlineSubsystemMcp*, void*, void*, void*, FString&);

public:

	static void PostInit();

	static void Init();
};