#pragma once
#include "Curl/CurlHttp.h"

class FCurlHttpRequestPatch
{
private:

	static bool ProcessRequestHook(FCurlHttpRequest*);

public:

	static void Init();
};