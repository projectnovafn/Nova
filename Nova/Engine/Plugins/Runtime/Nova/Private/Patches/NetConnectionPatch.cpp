#include "Patches/NetConnectionPatch.h"
#include "KismetDetoursLibrary.h"

static void (*_SetClientLoginState)(UNetConnection*, EClientLoginState);

UDetour* SetClientLoginStateDetour = NULL;

void UNetConnectionPatch::SetClientLoginStateHook(UNetConnection* Connection, EClientLoginState State)
{
	Connection->SetClientLoginState(State);

	_SetClientLoginState(Connection, State);
}

void UNetConnectionPatch::Init()
{
	_SetClientLoginState = UKismetMemoryLibrary::Get<decltype(_SetClientLoginState)>(L"UNetConnection::SetClientLoginState");

	SetClientLoginStateDetour = new UDetour();
	SetClientLoginStateDetour->Init(_SetClientLoginState, UNetConnectionPatch::SetClientLoginStateHook);
	SetClientLoginStateDetour->Commit();
}