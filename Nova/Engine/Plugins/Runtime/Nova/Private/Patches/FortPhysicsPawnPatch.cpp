#include "Patches/FortPhysicsPawnPatch.h"
#include "KismetDetoursLibrary.h"

static void (*_ServerMove)(AFortPhysicsPawn*, FFrame&);

void AFortPhysicsPawnPatch::ServerMoveHook(AFortPhysicsPawn* Pawn, FFrame& Stack)
{
	Pawn->ServerMove(*(FReplicatedPhysicsPawnState*)Stack.Locals);
}

void AFortPhysicsPawnPatch::Init()
{
	UKismetDetoursLibrary::AddReflectedDetour<AFortPhysicsPawn>(
		L"ServerMove",
		EReflectedDetourType::ExecSwap,
		ServerMoveHook,
		_ServerMove
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortPhysicsPawn>(
		L"ServerUpdatePhysicsParams",
		EReflectedDetourType::ExecSwap,
		ServerMoveHook,
		_ServerMove
	);
}