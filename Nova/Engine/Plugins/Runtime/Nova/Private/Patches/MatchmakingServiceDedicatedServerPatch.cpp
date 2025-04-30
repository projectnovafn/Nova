#include "Patches/MatchmakingServiceDedicatedServerPatch.h"
#include "KismetDetoursLibrary.h"

static void (*_HandleWebSocketMessage)(FMatchmakingServiceDedicatedServer*, FString&);
static void (*_HandleWebSocketError)(FMatchmakingServiceDedicatedServer*, void*);

UDetour* HandleWebSocketMessageDetour = NULL;
UDetour* HandleWebSocketErrorDetour = NULL;

void FMatchmakingServiceDedicatedServerPatch::HandleWebSocketMessageHook(FMatchmakingServiceDedicatedServer* MatchmakingService, FString& Message)
{
	MatchmakingService->HandleWebSocketMessage(Message);

	_HandleWebSocketMessage(MatchmakingService, Message);
}

void FMatchmakingServiceDedicatedServerPatch::HandleWebSocketErrorHook(FMatchmakingServiceDedicatedServer* MatchmakingService, void* a2)
{
	MatchmakingService->HandleWebSocketError(a2);

	_HandleWebSocketError(MatchmakingService, a2);
}

void FMatchmakingServiceDedicatedServerPatch::Init()
{
	_HandleWebSocketMessage = UKismetMemoryLibrary::Get<decltype(_HandleWebSocketMessage)>(L"FMatchmakingServiceDedicatedServer::HandleWebSocketMessage");
	_HandleWebSocketError = UKismetMemoryLibrary::Get<decltype(_HandleWebSocketError)>(L"FMatchmakingServiceDedicatedServer::HandleWebSocketError");

	if (auto Finalize = UKismetMemoryLibrary::Get<uint8*>(L"FMatchmakingServiceDedicatedServer::Finalize"))
		*Finalize = 0xC3;

	HandleWebSocketMessageDetour = new UDetour();
	HandleWebSocketMessageDetour->Init(_HandleWebSocketMessage, HandleWebSocketMessageHook);
	//HandleWebSocketMessageDetour->Commit();
	
	HandleWebSocketErrorDetour = new UDetour();
	HandleWebSocketErrorDetour->Init(_HandleWebSocketError, HandleWebSocketErrorHook);
	HandleWebSocketErrorDetour->Commit();
}