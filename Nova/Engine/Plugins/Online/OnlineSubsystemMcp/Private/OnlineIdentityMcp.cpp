#include "OnlineIdentityMcp.h"

bool FOnlineIdentityMcp::AutoLogin(int32 LocalUserNum)
{
	AddUserAccount(LocalUserNum, L"Test123", L"DedicatedServer");

	return AuthClient(GetClientId());
}

bool FOnlineIdentityMcp::AuthClient(FString ClientId)
{
	static auto AuthClient = UKismetMemoryLibrary::Get<bool (*)(FOnlineIdentityMcp*, FString&)>(L"FOnlineIdentityMcp::AuthClient");

	return AuthClient(this, ClientId);
}

void FOnlineIdentityMcp::AddUserAccount(int32 LocalUserNum, FString UserIdStr, FString AuthTokenStr)
{
	static auto AddUserAccount = UKismetMemoryLibrary::Get<void (*)(FOnlineIdentityMcp*, int32, FString&, FString&)>(L"FOnlineIdentityMcp::AddUserAccount");

	AddUserAccount(this, LocalUserNum, UserIdStr, AuthTokenStr);
}

FString FOnlineIdentityMcp::GetClientId()
{
	return L"59a0d47086f24d7b9c97997aa61c736e";
}

FString FOnlineIdentityMcp::GetClientSecret()
{
	return L"f3f3eb453e1b41ba8e382f480940ce6d";
}