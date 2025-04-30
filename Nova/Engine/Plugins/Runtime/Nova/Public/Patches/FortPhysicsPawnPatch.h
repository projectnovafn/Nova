#pragma once
#include "Pawns/FortPhysicsPawn.h"
#include "UObject/Stack.h"

class AFortPhysicsPawnPatch
{
private:
	static void ServerMoveHook(AFortPhysicsPawn*, FFrame&);

public:

	static void Init();
};