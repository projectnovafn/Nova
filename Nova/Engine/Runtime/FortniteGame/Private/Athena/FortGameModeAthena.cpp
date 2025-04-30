#include "Athena/FortGameModeAthena.h"
#include "AbilitySystemComponent.h"

#include "Kismet/FortKismetLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Engine/GameplayStatics.h"

#include "Building/BuildingFoundation.h"
#include "Building/BuildingGameplayActorSpawnMachine.h"

#include "Athena/Online/FortGameSessionDedicatedAthena.h"
#include "Online/FortReplicationGraph.h"

#include "Athena/FortAthenaVehicleSpawner.h"

#include "Athena/AI/AthenaNavSystem.h"

#include "Athena/Creative/FortCreativePortalManager.h"

#include "VivoxVoiceChat.h"

#include "UACServer.h"

#include "Nova.h"

void AFortGameModeAthena::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogFort, Log, TEXT("PostLogin: %s"), *NewPlayer->GetName());

	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);
	auto PlayerController = Cast<AFortPlayerControllerAthena>(NewPlayer);
	auto PlayerState = Cast<AFortPlayerStateAthena>(NewPlayer->PlayerState);

	if (!GameState || !PlayerController || !PlayerState)
		return;

	if (GameState->WarmupCountdownStartTime == -1 && GNova->bIsLateGame)
	{
		GameState->WarmupCountdownStartTime = UGameplayStatics::GetTimeSeconds(GetWorld());
		GameState->WarmupCountdownEndTime = GameState->WarmupCountdownStartTime + 25.0f;
	}

	if (auto PortalManager = GameState->CreativePortalManager)
	{
		PortalManager->AssignPortal(PlayerController);
	}

	static auto PlayerId = 0;
	PlayerState->WorldPlayerId = PlayerId++;

	if (FGameMemberInfoArray::StaticStruct() != NULL)
	{
		static auto MemberInfoProp = FGameMemberInfoArray::StaticStruct()->GetProperty(L"Members");

		UKismetArrayLibrary::Array_Add(GameState->GameMemberInfoArray.Members, MemberInfoProp, FGameMemberInfo::Construct(PlayerState->TeamIndex, PlayerState->SquadId, PlayerState->UniqueId));
		GameState->GameMemberInfoArray.MarkArrayDirty();
	}

	if (auto NetDriver = GetWorld()->NetDriver)
	{
		if (auto ReplicationGraph = Cast<UFortReplicationGraph>(NetDriver->ReplicationDriver))
		{
			ReplicationGraph->OnPlayerStateSquadAssign(PlayerState);
		}
	}

	if (auto AbilitySystemComponent = PlayerState->AbilitySystemComponent)
	{
		auto AthenaPlayer = LoadObject<UFortAbilitySet>(L"/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_AthenaPlayer.GAS_AthenaPlayer");
		auto AbilitySet = AthenaPlayer ? AthenaPlayer : LoadObject<UFortAbilitySet>(L"/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_DefaultPlayer.GAS_DefaultPlayer");

		for (auto& GameplayAbility : AbilitySet->GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(GameplayAbility);
		}

		for (auto& Modifier : GameState->GetCurrentPlaylist()->ModifierList)
		{
			for (auto& PersistentAbilitySet : Modifier->PersistentAbilitySets)
			{
				for (auto& AbilitySet : PersistentAbilitySet.AbilitySets)
				{
					for (auto& GameplayAbility : AbilitySet->GameplayAbilities)
					{
						AbilitySystemComponent->GiveAbility(GameplayAbility);
					}
				}
			}
		}
	}

	if (auto WorldInventory = PlayerController->WorldInventory)
	{
		for (auto& Item : StartingItems)
		{
			WorldInventory->AddItem(Item.Item, Item.Count);
		}

		WorldInventory->Update();
	}
}

void AFortGameModeAthena::Tick()
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (!GameState)
		return;

	for (auto& Player : GameState->PlayerArray)
	{
		auto PlayerState = Cast<AFortPlayerStateAthena>(Player);

		if (!PlayerState)
			continue;

		auto PlayerController = Cast<AFortPlayerControllerAthena>(Player->GetOwner());

		if (!PlayerController)
			continue;

		PlayerController->Tick();
	}
}

void AFortGameModeAthena::OnSafeZonePhaseChanged(int Phase)
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (SafeZonePhase >= 5 && GameState->PlayerArray.Num() >= 35)
	{
		for (auto& Player : GameState->PlayerArray)
		{
			auto PlayerState = Cast<AFortPlayerStateAthena>(Player);

			if (!PlayerState)
				continue;

			PlayerState->StormSurgeEffectCount++;
			PlayerState->OnRep_StormSurgeEffectCount();
		}
	}
}

int AFortGameModeAthena::GetLateSafeZoneIndex()
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (!GameState)
		return 3;

	return GameState->PlayerArray.Num() >= 25 ? 3 : 4;
}

FName AFortGameModeAthena::RedirectLootTierGroup(const FName& TierGroup)
{
	static FName Loot_TreasureFName = L"Loot_Treasure";
	static FName Loot_AmmoFName = L"Loot_Ammo";

	if (TierGroup == Loot_TreasureFName)
		return L"Loot_AthenaTreasure";

	if (TierGroup == Loot_AmmoFName)
		return L"Loot_AthenaAmmoLarge";

	return TierGroup;
}

void AFortGameModeAthena::HandleMatchHasStarted()
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (!GameState)
		return;

	if (auto PawnClass = LoadObject<UClass>(L"/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C"))
	{
		DefaultPawnClass = PawnClass;
	}

	auto Spawners = UGameplayStatics::GetAllActorsOfClass<AFortAthenaVehicleSpawner>(GetWorld());

	for (auto& Spawner : Spawners)
	{
		Spawner->SpawnVehicle();
	}

	auto RebootVans = UGameplayStatics::GetAllActorsOfClass<ABuildingGameplayActorSpawnMachine>(GetWorld());

	for (auto& RebootVan : RebootVans)
	{
		RebootVan->Setup();
	}

	if (auto PortalManager = GameState->CreativePortalManager)
	{
		PortalManager->Setup();

		UFortPlaysetItemDefinition::Setup();
	}

#ifdef STAGING
	if (auto NavigationSystem = Cast<UAthenaNavSystem>(GetWorld()->NavigationSystem))
	{
		UE_LOG(LogFort, Log, TEXT("Building NavSystem via UAthenaNavSystem"));

		// NavigationSystem->Build();
	}
	else
	{
		UE_LOG(LogFort, Warning, TEXT("Failed to retrieve UAthenaNavSystem"));
	}
#endif
}

bool AFortGameModeAthena::ReadyToStartMatch()
{
	GWorld = GetWorld();

	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (!GameState)
		return false;

	if (!GameState->MapInfo)
		return false;

	bWorldIsReady = true;

	for (auto& Player : GameState->PlayerArray)
	{
		auto PlayerState = Cast<AFortPlayerStateAthena>(Player);

		if (!PlayerState)
			continue;

		auto PlayerController = Cast<AFortPlayerControllerAthena>(Player->GetOwner());

		if (!PlayerController)
			continue;

		if (PlayerController->bHasServerFinishedLoading)
		{
			UFortKismetLibrary::SpawnDynamicItems(GetWorld());

			return true;
		}
	}

	return false;
}

bool AFortGameModeAthena::ReadyToEndMatch()
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (!GameState)
		return false;

	return !GameState->PlayersLeft;
}

void AFortGameModeAthena::InitGame()
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (!GameState)
		return;

	TArray<FString> Foundations =
	{
		L"/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.ShopsNew",
		L"/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Athena_POI_25x36",
		L"/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Athena_StreamingTest16",
		UKismetSystemLibrary::GetFortniteVersion() == 10.40 ? 
		L"/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.SLAB_1" :
		L"/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.SLAB_2",
		L"/Game/Athena/Apollo/Maps/Apollo_POI_Foundations.Apollo_POI_Foundations.PersistentLevel.Lobby_Foundation",
		L"/Game/Athena/Apollo/Maps/Apollo_POI_Foundations.Apollo_POI_Foundations.PersistentLevel.LF_Athena_POI_19x19_2",
		L"/Game/Athena/Apollo/Maps/Apollo_POI_Foundations.Apollo_POI_Foundations.PersistentLevel.BP_Jerky_Head6_18",
		L"/Game/Athena/Apollo/Maps/Apollo_POI_Foundations.Apollo_POI_Foundations.PersistentLevel.BP_Jerky_Head5_14",
		L"/Game/Athena/Apollo/Maps/Apollo_POI_Foundations.Apollo_POI_Foundations.PersistentLevel.BP_Jerky_Head_2",
		L"/Game/Athena/Apollo/Maps/Apollo_POI_Foundations.Apollo_POI_Foundations.PersistentLevel.BP_Jerky_Head4_11",
		L"/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.PleasentParkFestivus",
		L"/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Athena_POI_50x53_Volcano"
	};

	for (auto& FoundationPath : Foundations)
	{
		auto Foundation = FindObject<ABuildingFoundation>(*FoundationPath);

		if (!Foundation || Foundation->IsShown())
			continue;

		Foundation->Show();
	}
}

UClass* AFortGameModeAthena::GetGameSessionClass()
{
	return AFortGameSessionDedicatedAthena::StaticClass();
}

void AFortGameModeAthena::AddToAlivePlayers(AFortPlayerControllerAthena* PlayerController)
{
	static auto AddToAlivePlayers = UKismetMemoryLibrary::Get<void (*)(AFortGameModeAthena*, AFortPlayerControllerAthena*)>(L"AFortGameModeAthena::AddToAlivePlayers");

	AddToAlivePlayers(this, PlayerController);
}

void AFortGameModeAthena::RemoveFromAlivePlayers(AFortPlayerControllerAthena* PlayerController, APlayerState* KillerPlayerState, APawn* KillerPawn, UFortWeaponItemDefinition* KillerWeapon, uint8 DeathCause, bool bDBNO)
{
	static auto RemoveFromAlivePlayers = UKismetMemoryLibrary::Get<void (*)(AFortGameModeAthena*, AFortPlayerControllerAthena*, APlayerState*, APawn*, UFortWeaponItemDefinition*, uint8, bool)>(L"AFortGameModeAthena::RemoveFromAlivePlayers");

	RemoveFromAlivePlayers(this, PlayerController, KillerPlayerState, KillerPawn, KillerWeapon, DeathCause, bDBNO);
}

bool AFortGameModeAthena::PlayerCanRestart(APlayerController* Player, PlayerCanRestartType Callback)
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (auto FortPC = Cast<AFortPlayerControllerAthena>(Player))
	{
		if (auto PlayerState = Cast<AFortPlayerStateAthena>(FortPC->PlayerState))
		{
			if (PlayerState->bInAircraft && !FortPC->Pawn && !GameState->bAircraftIsLocked)
				return true;
		}
	}

	return Callback(this, Player);
}

APawn* AFortGameModeAthena::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot)
{
	UE_LOG(LogFort, Log, TEXT("SpawnDefaultPawnFor: %s, %s"), *NewPlayer->GetName(), *StartSpot->GetName());

	return GetWorld()->SpawnActor<APawn>(StartSpot->GetActorLocation(), StartSpot->GetActorRotation(), DefaultPawnClass);
}

void AFortGameModeAthena::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	struct
	{
		AController* NewPlayer;
		AActor* StartSpot;
	} params(NewPlayer, StartSpot);

	this->ProcessEvent(L"RestartPlayerAtPlayerStart", &params);
}

AFortPlayerControllerAthena* AFortGameModeAthena::PickSpectatingTarget(AFortPlayerControllerAthena* PlayerController)
{
	auto PlayerState = Cast<AFortPlayerStateAthena>(PlayerController->PlayerState);

	for (auto TeamMember : PlayerState->PlayerTeam->TeamMembers)
	{
		auto FortTeamMember = Cast<AFortPlayerControllerAthena>(TeamMember);
		if (FortTeamMember == PlayerController) continue;

		if (!FortTeamMember->bMarkedAlive)
		{
			auto PlayerState = Cast<AFortPlayerStateAthena>(FortTeamMember->PlayerState);

			if (auto SpectatingTarget = PlayerState->SpectatingTarget)
			{
				return Cast<AFortPlayerControllerAthena>(SpectatingTarget->GetOwner());
			}

			continue;
		}

		return FortTeamMember;
	}

	if (auto KillerPlayerState =
		Cast<AFortPlayerStateAthena>(PlayerState->DeathInfo.FinisherOrDowner->IsValid() ? PlayerState->DeathInfo.FinisherOrDowner : NULL))
	{
		return Cast<AFortPlayerControllerAthena>(KillerPlayerState->GetOwner());
	}

	return AlivePlayers.Num() > 0 ? AlivePlayers[FMath::Rand() % AlivePlayers.Num()] : NULL;
}

bool AFortGameModeAthena::GenerateVoiceChatToken(AFortPlayerControllerZone* PlayerController, EVoiceChannelType ChannelType, FString const& ChannelName, FString& Token, FString& Error)
{
	auto VivoxVoiceChat = FVivoxVoiceChat::Get();

	if (!VivoxVoiceChat)
		return false;

	auto PlayerState = PlayerController->PlayerState;

	if (!PlayerState)
		return false;

	Token = VivoxVoiceChat->GetJoinToken(*PlayerController->PlayerState->UniqueId, ChannelName);

	return true;
}