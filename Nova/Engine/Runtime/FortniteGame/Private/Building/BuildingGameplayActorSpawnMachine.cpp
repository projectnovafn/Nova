#include "Building/BuildingGameplayActorSpawnMachine.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/FortKismetLibrary.h"
#include "Engine/GameplayStatics.h"

#include "Athena/FortGameModeAthena.h"
#include "Athena/FortGameStateAthena.h"
#include "Components/AthenaResurrectionComponent.h"
#include "Pawns/FortPlayerPawnAthena.h"
#include "Kismet/FortKismetLibrary.h"

void ABuildingGameplayActorSpawnMachine::OnRep_SpawnMachineState()
{
	this->ProcessEvent(L"OnRep_SpawnMachineState");
}

void ABuildingGameplayActorSpawnMachine::Setup()
{
	auto PlayerStarts = UGameplayStatics::GetAllActorsOfClass<AFortPlayerStart>(GetWorld());

    ResurrectLocation = UFortKismetLibrary::GetClosestActorFromArray(this, PlayerStarts);

    UE_LOG(LogFort, Log, TEXT("ABuildingGameplayActorSpawnMachine::Setup: %s"), *ResurrectLocation->GetName());
}

void ABuildingGameplayActorSpawnMachine::ResurrectPlayer()
{
	UE_LOG(LogFort, Log, TEXT("ABuildingGameplayActorSpawnMachine::ResurrectPlayer: %s, %i"), *ResurrectLocation->GetName(), PlayerIdsForResurrection.Num());

	if (!ResurrectLocation)
		return;

    auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->AuthorityGameMode);
    auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

    if (!PlayerIdsForResurrection.Num())
    {
        auto PlayerState = Cast<AFortPlayerStateAthena>(InstigatorPC->PlayerState);

        for (auto TeamMember : PlayerState->PlayerTeam->TeamMembers)
        {
            auto PlayerController = Cast<AFortPlayerControllerAthena>(TeamMember);

            if (!PlayerController || !PlayerController->ResurrectionComponent) continue;

            PlayerController->ResurrectionComponent->PlayerIdsForResurrection.Empty();
            PlayerController->ResurrectionComponent->OnRep_PlayerIdsForResurrection();
        }

        return OnResurrectionCompleted();
    }

    auto PlayerState = Cast<AFortPlayerStateAthena>(GameState->FindPlayerById(PlayerIdsForResurrection[0]));

    if (!PlayerState)
        return;

    auto PlayerController = Cast<AFortPlayerControllerAthena>(PlayerState->GetOwner());

    FMemory::Memzero(&PlayerState->RespawnData, FFortRespawnData::StaticStruct()->PropertiesSize);
    FMemory::Memzero(&PlayerController->PlayerDeathReport, FFortPlayerDeathReport::StaticStruct()->PropertiesSize);

    PlayerController->ResurrectionComponent->ResurrectionLocation = ResurrectLocation;
    PlayerController->ServerSetSpectatorWaiting(true);
    PlayerController->RespawnPlayerAfterDeath(false);

    PlayerState->RebootCounter++;
    PlayerState->OnRep_RebootCounter();

    GameMode->AddToAlivePlayers(PlayerController);

    PlayerController->bMarkedAlive = true;

    PlayerController->ClientOnPawnSpawned();
    PlayerController->ClientClearDeathNotification();

    if (auto Pawn = Cast<AFortPlayerPawnAthena>(PlayerController->Pawn))
    {
        PlayerController->ClientOnPawnRevived(InstigatorPC.Get());

        OnPlayerPawnResurrected(Pawn);
    }

    PlayerIdsForResurrection.RemoveAt(0);

    QueueTask(ENamedThreads::GameThread, PlayerIdsForResurrection.Num() ? 2.5f : 0.1f, [this] { this->ResurrectPlayer(); });
}

void ABuildingGameplayActorSpawnMachine::OnPlayerPawnResurrected(AFortPlayerPawn* PlayerPawn)
{
    this->ProcessEvent(L"OnPlayerPawnResurrected", &PlayerPawn);
}

void ABuildingGameplayActorSpawnMachine::OnResurrectionCompleted()
{
    this->ProcessEvent(L"OnResurrectionCompleted");
}