#pragma once
#include "CoreUObject.h"

class FOnlineIdentityMcp
{
public:

	bool AutoLogin(int32 LocalUserNum);
	bool AuthClient(FString ClientId);
	void AddUserAccount(int32 LocalUserNum, FString UserIdStr, FString AuthTokenStr);

	static FString GetClientId();
	static FString GetClientSecret();
};