#include "Patches/FortControllerComponent_InteractionPatch.h"
#include "KismetDetoursLibrary.h"

static void (*_execServerAttemptInteract)(UFortControllerComponent_Interaction*, FFrame&);

void UFortControllerComponent_InteractionPatch::execServerAttemptInteractHook(UFortControllerComponent_Interaction* InteractionComp, FFrame& Stack)
{
	InteractionComp->ServerAttemptInteract(*(AActor**)Stack.Locals);

	_execServerAttemptInteract(InteractionComp, Stack);
}

void UFortControllerComponent_InteractionPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<UFortControllerComponent_Interaction>(
		L"ServerAttemptInteract",
		EReflectedDetourType::ExecSwap,
		execServerAttemptInteractHook,
		_execServerAttemptInteract
	);
}