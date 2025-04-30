#include "Patches/FortGameSessionDedicatedPatch.h"
#include "KismetDetoursLibrary.h"
#include "GameFramework/GameMode.h"

#include "Nova.h"

static void (*_HandleMatchAssignment)(AActor*, FMatchmakingDedicatedServerMatchAssignment&);
static void (*_Restart)(AFortGameSessionDedicated*);

UDetour* HandleMatchAssignmentDetour = NULL;
UDetour* RestartDetour = NULL;

void AFortGameSessionDedicatedPatch::HandleMatchAssignmentHook(AActor* PartyBeacon, FMatchmakingDedicatedServerMatchAssignment& Assignment)
{
	if (auto GameSession = Cast<AFortGameSessionDedicated>(PartyBeacon->GetWorld()->AuthorityGameMode->GameSession))
		GameSession->HandleMatchAssignment(Assignment);

	_HandleMatchAssignment(PartyBeacon, Assignment);
}

void AFortGameSessionDedicatedPatch::RestartHook(AFortGameSessionDedicated* GameSession)
{
	GameSession->Restart();

	_Restart(GameSession);
}

void AFortGameSessionDedicatedPatch::Init()
{
	_HandleMatchAssignment = UKismetMemoryLibrary::Get<decltype(_HandleMatchAssignment)>(L"AFortPartyBeaconHost::HandleMatchAssignment");
	_Restart = UKismetMemoryLibrary::Get<decltype(_Restart)>(L"AFortGameSessionDedicated::Restart");

	HandleMatchAssignmentDetour = new UDetour();
	HandleMatchAssignmentDetour->Init(_HandleMatchAssignment, HandleMatchAssignmentHook);
	HandleMatchAssignmentDetour->Commit();

	RestartDetour = new UDetour();
	RestartDetour->Init(_Restart, RestartHook);
	RestartDetour->Commit();
}