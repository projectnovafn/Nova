#include "Athena/Creative/FortAthenaCreativePortal.h"

#include "Pawns/FortPlayerPawnAthena.h"
#include "Athena/Player/FortPlayerControllerAthena.h"

#include "Kismet/FortKismetLibrary.h"

void AFortAthenaCreativePortal::TeleportPlayerToLinkedVolume(AFortPlayerPawnAthena* Pawn, bool bUseSpawnTags)
{
	if (!Pawn || !LinkedVolume)
		return;

	auto PlayerController = Cast<AFortPlayerControllerAthena>(Pawn->Controller);

	if (!PlayerController)
		return;

	auto Location = LinkedVolume->GetActorLocation();
	auto Rotation = LinkedVolume->GetActorRotation();

	Location = UFortKismetLibrary::FindStaticGroundLocationAt(GetWorld(), Location, NULL, 10000.0f, 0.0f);
	Location.Z += 500;

	if (Pawn->TeleportTo(Location, Rotation))
	{
		Pawn->TeleportToSkyDive(15000.0f);

		OnPlayerPawnTeleported(Pawn, false);
	}
}

void AFortAthenaCreativePortal::OnPlayerPawnTeleported(AFortPlayerPawn* PlayerPawn, bool bTeleportedToIslandStart)
{
	struct Params
	{
		AFortPlayerPawn* PlayerPawn;
		bool bTeleportedToIslandStart;
	} params = { PlayerPawn, bTeleportedToIslandStart };

	this->ProcessEvent(L"OnPlayerPawnTeleported", &params);
}

void AFortAthenaCreativePortal::OnRep_PortalOpen()
{
	this->ProcessEvent(L"OnRep_PortalOpen");
}

void AFortAthenaCreativePortal::Setup()
{
	UE_LOG(LogFort, Log, TEXT("AFortAthenaCreativePortal::Setup: %i, %s, %s, [%f, %f, %f]"), PortalIndex,
		*DestinationActor->GetName(), *LinkedVolume->GetName(), TeleportLocation.X, TeleportLocation.Y, TeleportLocation.Z);
}