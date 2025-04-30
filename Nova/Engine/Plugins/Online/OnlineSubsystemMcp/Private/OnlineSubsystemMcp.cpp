#include "OnlineSubsystemMcp.h"

bool FOnlineSubsystemMcp::ProcessRequestAsUser(void* McpService, void* UserId, void* HttpRequest, FString& Error)
{
	UE_LOG(LogOnline, Log, TEXT("FOnlineSubsystemMcp::ProcessRequestAsUser"));

	return ProcessRequestAsClient(McpService, FOnlineIdentityMcp::GetClientId(), HttpRequest, Error);
}

bool FOnlineSubsystemMcp::ProcessRequestAsClient(void* McpService, FString ClientId, void* HttpRequest, FString& Error)
{
	static auto ProcessRequestAsClient = UKismetMemoryLibrary::Get<bool (*)(FOnlineSubsystemMcp*, void*, FString&, void*, FString&)>(L"FOnlineSubsystemMcp::ProcessRequestAsClient");

	return ProcessRequestAsClient(this, McpService, ClientId, HttpRequest, Error);
}