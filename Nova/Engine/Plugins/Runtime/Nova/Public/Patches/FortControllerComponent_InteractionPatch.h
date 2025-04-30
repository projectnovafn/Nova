#pragma once
#include "Components/FortControllerComponent_Interaction.h"
#include "UObject/Stack.h"

class UFortControllerComponent_InteractionPatch
{
private:
	static void execServerAttemptInteractHook(UFortControllerComponent_Interaction*, FFrame&);

public:

	static void Init();
};