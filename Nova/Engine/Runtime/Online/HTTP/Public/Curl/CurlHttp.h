#pragma once

#include "CoreMinimal.h"
#include "HAL/ThreadSafeCounter.h"

#ifdef PRODUCTION
constexpr auto API_URL = L"https://api.novafn.dev";
#else
constexpr auto API_URL = L"https://api-staging.novafn.dev";
#endif

class FCurlHttpRequest
{
private:

	void** VTable;

public:

	FString GetURL()
	{
		FString Result;
		return ((FString& (*)(FCurlHttpRequest*, FString&))(*VTable))(this, Result);
	}

	void SetURL(FString URL)
	{
		((void (*)(FCurlHttpRequest*, FString&))(VTable[10]))(this, URL);
	}

	bool ProcessRequest();
};