#pragma once
#include "Engine/NetConnection.h"

class UNetConnectionPatch
{
private:
	static void SetClientLoginStateHook(UNetConnection*, EClientLoginState);

public:

	static void Init();
};