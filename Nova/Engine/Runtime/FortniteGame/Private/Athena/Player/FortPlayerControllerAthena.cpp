#include "Athena/Player/FortPlayerControllerAthena.h"
#include "Athena/Player/FortPlayerStateAthena.h"
#include "Pawns/FortPlayerPawnAthena.h"

#include "Engine/World.h"
#include "Engine/GameplayStatics.h"

#include "Athena/FortGameModeAthena.h"
#include "Athena/Creative/FortPlayerStartCreative.h"

#include "Projectiles/FortProjectileBase.h"

#include "Kismet/FortKismetLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetTextLibrary.h"

#include "Missions/FortMissionLibrary.h"

#include "ItemDefinitions/FortCreativeRealEstatePlotItem.h"

#include "Athena/Creative/FortAthenaCreativePortal.h"
#include "Athena/Creative/FortCreativePortalManager.h"

#include "Nova.h"

void AFortPlayerControllerAthena::ApplySiphon()
{
	if (!MyFortPawn)
		return;
	
	auto AbilitySystemComponent = MyFortPawn->AbilitySystemComponent;

	if (!AbilitySystemComponent)
		return;

	AbilitySystemComponent->InvokeGameplayCueExecuted(FGameplayTag(L"GameplayCue.Shield.PotionConsumed"), {}, {});

	auto MaxHealth = MyFortPawn->GetMaxHealth();
	auto MaxShield = MyFortPawn->GetMaxShield();

	auto CurrentHealth = MyFortPawn->GetHealth();
	auto CurrentShield = MyFortPawn->GetShield();

	if (CurrentHealth + 50 > MaxHealth)
	{
		MyFortPawn->SetHealth(MaxHealth);

		if (CurrentShield + CurrentHealth + 50 - MaxHealth > MaxShield)
			MyFortPawn->SetShield(MaxShield);
		else
			MyFortPawn->SetShield(CurrentShield + CurrentHealth + 50 - MaxHealth);
	}
	else
	{
		MyFortPawn->SetHealth(CurrentHealth + 50);
	}
}

void AFortPlayerControllerAthena::ServerAcknowledgePossession(APawn* P)
{
	UE_LOG(LogFort, Log, TEXT("ServerAcknowledgePossession: %s"), *P->GetName());

	AcknowledgedPawn = P;

	auto PlayerState = Cast<AFortPlayerStateAthena>(this->PlayerState);

	if (!PlayerState)
		return;

	auto Pawn = Cast<AFortPlayerPawn>(P);

	if (!Pawn)
		return;

	PlayerState->ApplyCharacterCustomization(Pawn);

	if (!bHasInitializedWorldInventory)
	{
		bHasInitializedWorldInventory = true;

		if (WorldInventory->AddItem(GetCosmeticLoadout().Pickaxe->WeaponDefinition, 1))
		{
			WorldInventory->Update();

			ServerExecuteInventoryItem(WorldInventory->GetPickaxeItem());
		}
	}
}

void AFortPlayerControllerAthena::ServerSetRespawnData(FVector& Location, FRotator& Rotation)
{
	struct Params { FVector Location; FRotator Rotation; } params(Location, Rotation);

	this->ProcessEvent(L"ServerSetRespawnData", &params);
}

void AFortPlayerControllerAthena::ClientStartRespawnPreparation(FVector& RespawnLoc, FRotator& RespawnRot, float RespawnCameraDist, FName InRespawnCameraBehavior, FText& HUDReasonText)
{
	struct Params { FVector RespawnLoc; FRotator RespawnRot; float RespawnCameraDist; FName InRespawnCameraBehavior; FText HUDReasonText; } params(RespawnLoc, RespawnRot, RespawnCameraDist, InRespawnCameraBehavior, HUDReasonText);;
	this->ProcessEvent(L"ClientStartRespawnPreparation", &params);
}

void AFortPlayerControllerAthena::ServerChangeName(FString S)
{
	UE_LOG(LogFort, Log, TEXT("ServerChangeName: %s"), *S);
}

bool AFortPlayerControllerAthena::ServerAttemptAircraftJump(FRotator ClientRotation, bool Force)
{
	if (Pawn || (!GNova->bIsLateGame && !Force))
		return true;

	auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->AuthorityGameMode);
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);
	auto Aircraft = GameState->GetAircraft(0);

	if (!Aircraft || GameState->bAircraftIsLocked)
		return true;

	GameMode->RestartPlayerAtPlayerStart(this, Aircraft);

	ClientSetRotation(ClientRotation, false);

	if (MyFortPawn && GNova->bIsLateGame)
	{
		MyFortPawn->SetHealth(100);
		MyFortPawn->SetShield(100);
	}

	return false;
}

void AFortPlayerControllerAthena::EnterAircraft(AFortAthenaAircraft* Aircraft)
{
	auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->AuthorityGameMode);
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	UE_LOG(LogFort, Log, TEXT("EnterAircraft: %s, %p"), *GetName(), WorldInventory);

	auto AthenaPlayerState = Cast<AFortPlayerStateAthena>(PlayerState);
	auto MatchAbandonStateProp = AthenaPlayerState->GetClassProperty(L"MatchAbandonState");

	if (MatchAbandonStateProp != NULL)
	{
		AthenaPlayerState->MatchAbandonState = EMatchAbandonState::TeamLocked;
		AthenaPlayerState->OnRep_MatchAbandonState(EMatchAbandonState::Joining);
	}

	if (WorldInventory)
		WorldInventory->Clear();

	if (GNova->bIsLateGame)
		this->EquipLoadout();

	if (Aircraft != NULL && GNova->bIsLateGame && !GNova->bHasSetupBus)
	{
		GNova->bHasSetupBus = true;

		auto& FlightInfo = Aircraft->FlightInfo;

		bool IsSmallZone = GameMode->GetLateSafeZoneIndex() == 4;
		auto OffsetDistance = IsSmallZone ? 10000.0f : 25000.0f;
		auto NewLocation = GameMode->SafeZoneLocations[GameMode->GetLateSafeZoneIndex()];
		auto OffsetRotation = FlightInfo.FlightStartRotation + FRotator(0, 180, 0);
		auto OffsetDirection = OffsetRotation.Vector();

		NewLocation += OffsetDirection * OffsetDistance;

		FlightInfo.FlightStartLocation = NewLocation;
		FlightInfo.FlightStartLocation.Z = 25000;
		FlightInfo.FlightSpeed /= IsSmallZone ? 8 : 4;
		FlightInfo.TimeTillDropEnd -= ((FlightInfo.TimeTillDropEnd - FlightInfo.TimeTillDropStart) / 2);
	}
}

void AFortPlayerControllerAthena::ExitAircraft()
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	if (GNova->bIsLateGame && !GNova->bHasStartedZone)
	{
		GNova->bHasStartedZone = true;

		GameState->GamePhase = EAthenaGamePhase::SafeZones;
		GameState->OnRep_GamePhase(EAthenaGamePhase::Count);
	}

	if (GNova->bIsLateGame && MyFortPawn)
	{
		MyFortPawn->SetShield(100);
	}
}

void AFortPlayerControllerAthena::ClientOnPawnDied(FFortPlayerDeathReport& DeathReport)
{
	UE_LOG(LogFort, Log, TEXT("ClientOnPawnDied: %s"), *GetName());

	auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->AuthorityGameMode);
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	auto DeadPawn = Cast<AFortPlayerPawnAthena>(Pawn);
	auto DeadPlayerState = Cast<AFortPlayerStateAthena>(PlayerState);

	auto KillerPawn = DeathReport.KillerPawn;
	auto KillerPlayerState = DeathReport.KillerPlayerState ? Cast<AFortPlayerStateAthena>(DeathReport.KillerPlayerState) : DeadPlayerState;

	if (KillerPlayerState && DeadPlayerState)
	{
		FMemory::Memzero(&DeadPlayerState->DeathInfo, FDeathInfo::StaticStruct()->PropertiesSize);

		DeadPlayerState->DeathInfo.FinisherOrDowner = KillerPlayerState;
		DeadPlayerState->DeathInfo.Distance = (DeadPawn && KillerPawn) ? FVector::Dist(DeadPawn->GetActorLocation(), KillerPawn->GetActorLocation()) : (DeadPawn && DeadPawn->GetClassProperty(L"LastFallDistance") != NULL) ? DeadPawn->LastFallDistance : 0.f;
		DeadPlayerState->DeathInfo.DeathLocation = DeadPawn ? DeadPawn->GetActorLocation() : FVector();
		if (FDeathInfo::StaticStruct()->GetField(L"DeathTags") != NULL)
			DeadPlayerState->DeathInfo.DeathTags = DeathReport.Tags;
		DeadPlayerState->DeathInfo.DeathCause = AFortPlayerStateAthena::ToDeathCause(DeathReport.Tags, false);
		DeadPlayerState->DeathInfo.bInitialized = true;

		DeadPlayerState->OnRep_DeathInfo();

		if (KillerPlayerState != DeadPlayerState)
		{
			KillerPlayerState->KillScore++;
			KillerPlayerState->OnRep_Kills();

			for (auto TeamMember : KillerPlayerState->PlayerTeam->TeamMembers)
			{
				auto TeamPlayerState = Cast<AFortPlayerStateAthena>(TeamMember->PlayerState);

				if (!TeamPlayerState) 
					continue;

				TeamPlayerState->TeamKillScore++;
				TeamPlayerState->OnRep_TeamKillScore();

				TeamPlayerState->TeamScore++;
				TeamPlayerState->OnRep_TeamScore();

				TeamPlayerState->TeamScorePlacement = GameState->Teams.Num() - 1;
				TeamPlayerState->OnRep_TeamScorePlacement();

				TeamPlayerState->ClientReportTeamKill(TeamPlayerState->TeamKillScore); // <- amount of kills or amount to add?
			}

			KillerPlayerState->ClientReportKill(DeadPlayerState);

			if (auto PlayerController = Cast<AFortPlayerControllerAthena>(KillerPlayerState->GetOwner()))
				PlayerController->ApplySiphon();
		}
	}

	UFortWeaponItemDefinition* KillerWeapon = NULL;
	if (auto DamageCauser = DeathReport.DamageCauser)
	{
		if (DamageCauser->IsA<AFortProjectileBase>())
		{
			auto Owner = Cast<AFortWeapon>(DamageCauser->GetOwner());
			KillerWeapon = Owner ? Owner->WeaponData : NULL;
		}

		if (auto Weapon = Cast<AFortWeapon>(DamageCauser))
		{
			KillerWeapon = Weapon->WeaponData;
		}
	}

	if (!GameState->IsRespawningAllowed(KillerPlayerState))
	{
		if (WorldInventory)
		{
			for (auto Item : WorldInventory->GetDroppableItems())
			{
				WorldInventory->RemoveItem(Item->ItemEntry.ItemGuid, Item->ItemEntry.Count);
				UFortKismetLibrary::SpawnPickup(GetWorld(), DeadPlayerState->DeathInfo.DeathLocation, Item->ItemEntry.ItemDefinition, Item->ItemEntry.Count, Item->ItemEntry.LoadedAmmo, this, EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::PlayerElimination);
			}

			WorldInventory->Update();
		}

		GameMode->RemoveFromAlivePlayers(this, KillerPlayerState, KillerPawn, KillerWeapon, DeadPlayerState->DeathInfo.DeathCause, false);

		if (GameMode->bAllowSpectateAfterDeath && UKismetSystemLibrary::GetEngineVersionNum() <= 4.21)
		{
			QueueTask(ENamedThreads::GameThread, 3.0f, [this]()
			{
				if (!this->IsValid())
					return;

				auto GameMode = Cast<AFortGameModeAthena>(GetWorld()->AuthorityGameMode);
				auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

				if (auto SpectatingTarget = GameMode->PickSpectatingTarget(this))
					this->PlayerToSpectateOnDeath = SpectatingTarget->Pawn;

				for (auto Player : GameState->PlayerArray)
				{
					auto SpectatingPlayer = Cast<AFortPlayerStateAthena>(Player);

					if (!SpectatingPlayer)
						continue;

					auto SpectatingPC = Cast<AFortPlayerControllerAthena>(SpectatingPlayer->GetOwner());

					if (!SpectatingPC || SpectatingPC->bMarkedAlive)
						continue;

					auto SpectatingTarget = SpectatingPlayer->SpectatingTarget;

					if (SpectatingTarget->IsValid())
					{
						auto SpectatingTargetPC = Cast<AFortPlayerControllerAthena>(SpectatingTarget->GetOwner());

						if (SpectatingTargetPC->IsValid())
						{
							if (SpectatingTargetPC->bMarkedAlive)
								continue;
						}
					}

					if (auto PlayerToSpectateOnDeath = SpectatingPC->PlayerToSpectateOnDeath)
					{
						SpectatingPC->SpectateOnDeath();

						SpectatingPlayer->SpectatingTarget = Cast<AFortPlayerStateAthena>(PlayerToSpectateOnDeath->PlayerState);
						SpectatingPlayer->OnRep_SpectatingTarget();
					}
					else
					{
						if (auto SpectatingTarget = GameMode->PickSpectatingTarget(this))
						{
							SpectatingPlayer->SpectatingTarget = Cast<AFortPlayerStateAthena>(SpectatingTarget->PlayerState);
							SpectatingPlayer->OnRep_SpectatingTarget();
						}
					}
				}
			});
		}

		int Place = GameState->GetAliveTeams();

		if (!DeadPlayerState->PlayerTeam->IsAlive())
		{
			for (auto TeamMember : DeadPlayerState->PlayerTeam->TeamMembers)
			{
				auto AthenaTeamMember = Cast<AFortPlayerControllerAthena>(TeamMember);
				auto TeamPlayerState = Cast<AFortPlayerStateAthena>(TeamMember->PlayerState);

				if (!TeamPlayerState || !AthenaTeamMember)
					continue;

				TeamPlayerState->Place = Place + 1;
				TeamPlayerState->OnRep_Place();

				AthenaTeamMember->ClientSendTeamStatsForPlayer(FAthenaMatchTeamStats
				{
					TeamPlayerState->Place,
					GameState->TotalPlayers
				});

				AthenaTeamMember->ClientSendEndBattleRoyaleMatchForPlayer(true, {});
			}
		}

		for (auto TeamInfo : GameState->Teams)
		{
			if (TeamInfo->IsAlive())
			{
				for (auto TeamMember : TeamInfo->TeamMembers)
				{
					auto AthenaTeamMember = Cast<AFortPlayerControllerAthena>(TeamMember);
					auto TeamPlayerState = Cast<AFortPlayerStateAthena>(TeamMember->PlayerState);

					if (!TeamPlayerState || !AthenaTeamMember)
						continue;

					TeamPlayerState->Place = Place;
					TeamPlayerState->OnRep_Place();
				}
			}
		}

		if (auto WinningPlayerState = Cast<AFortPlayerStateAthena>(GameState->WinningPlayerState))
		{
			for (auto Member : WinningPlayerState->PlayerTeam->TeamMembers)
			{
				auto AthenaTeamMember = Cast<AFortPlayerControllerAthena>(Member);
				auto TeamPlayerState = Cast<AFortPlayerStateAthena>(Member->PlayerState);

				TeamPlayerState->Place = 1;
				TeamPlayerState->OnRep_Place();

				AthenaTeamMember->ClientSendTeamStatsForPlayer(FAthenaMatchTeamStats
				{
					WinningPlayerState->Place,
					GameState->TotalPlayers
				});

				AthenaTeamMember->ClientSendEndBattleRoyaleMatchForPlayer(true, {});
			}
		}
	}
}

void AFortPlayerControllerAthena::ClientSendTeamStatsForPlayer(FAthenaMatchTeamStats TeamStats)
{
	this->ProcessEvent(L"ClientSendTeamStatsForPlayer", &TeamStats);
}

void AFortPlayerControllerAthena::ClientSendMatchStatsForPlayer(FAthenaMatchStats MatchStats)
{
	this->ProcessEvent(L"ClientSendMatchStatsForPlayer", &MatchStats);
}

void AFortPlayerControllerAthena::ClientSendEndBattleRoyaleMatchForPlayer(bool bSuccess, FAthenaRewardResult Result)
{
	struct
	{
		bool bSuccess;
		FAthenaRewardResult Result;
	} params(bSuccess, Result);

	this->ProcessEvent(L"ClientSendEndBattleRoyaleMatchForPlayer", &params);
}

void AFortPlayerControllerAthena::ServerSetSpectatorWaiting(bool bWaiting)
{
	this->ProcessEvent(L"ServerSetSpectatorWaiting", &bWaiting);
}

void AFortPlayerControllerAthena::RespawnPlayerAfterDeath(bool bEnterSkydiving)
{
	this->ProcessEvent(L"RespawnPlayerAfterDeath", &bEnterSkydiving);
}

void AFortPlayerControllerAthena::ServerGiveCreativeItem(FFortItemEntry* ItemEntry)
{
	auto WeaponDef = Cast<UFortWeaponItemDefinition>(ItemEntry->ItemDefinition);

	if (!WorldInventory)
		return;

	bool bNeedsUpdate;
	WorldInventory->AddItem(ItemEntry->ItemDefinition, ItemEntry->Count, bNeedsUpdate, UFortKismetLibrary::GetClipSize(WeaponDef));

	if (bNeedsUpdate)
		WorldInventory->Update();
}

void AFortPlayerControllerAthena::ServerTeleportToPlaygroundLobbyIsland()
{
	UE_LOG(LogFort, Log, TEXT("ServerTeleportToPlaygroundLobbyIsland: %s"), *GetName());

	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	FGameplayTagContainer PlaylistContextTags = GameState->GetAthenaPlaylistContextTags();
	if (!PlaylistContextTags.Contains(L"Athena.Creative"))
		return;

	auto PlayerStarts = UGameplayStatics::GetAllActorsOfClass<AFortPlayerStartCreative>(this);
	auto ValidPlayerStarts = PlayerStarts.FilterByPredicate([](AFortPlayerStartCreative* Start)
	{
		return Start->PlayerStartTags.Contains(L"Playground.LobbyIsland.Spawn");
	});

	if (ValidPlayerStarts.Num() > 0)
	{
		auto PlayerStart = ValidPlayerStarts[0];
		auto Location = PlayerStart->GetActorLocation();
		auto Rotation = PlayerStart->GetActorRotation();

		UFortMissionLibrary::TeleportPlayerPawn(MyFortPawn, Location, Rotation, true, true);
	}
	else
	{
		UE_LOG(LogFort, Warning, TEXT("ServerTeleportToPlaygroundLobbyIsland: No valid player start found for %s"), *GetName());
	}
}

void AFortPlayerControllerAthena::ServerTeleportToPlaygroundIslandDock()
{
	UE_LOG(LogFort, Log, TEXT("ServerTeleportToPlaygroundIslandDock: %s"), *GetName());

	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);

	FGameplayTagContainer PlaylistContextTags = GameState->GetAthenaPlaylistContextTags();
	if (!PlaylistContextTags.Contains(L"Athena.Creative"))
		return;

	auto PlayerStarts = CreativePlotLinkedVolume->GetActorsWithinVolumeByClass<AFortPlayerStartCreative>();
	auto ValidPlayerStarts = PlayerStarts.FilterByPredicate([](AFortPlayerStartCreative* Start)
	{
		return Start->bUseAsIslandStart && Start->bIsEnabled;
	});

	if (ValidPlayerStarts.Num() > 0)
	{
		auto PlayerStart = ValidPlayerStarts[0];
		auto Location = PlayerStart->GetActorLocation();
		auto Rotation = PlayerStart->GetActorRotation();

		UFortMissionLibrary::TeleportPlayerPawn(MyFortPawn, Location, Rotation, true, true);
	}
	else
	{
		UE_LOG(LogFort, Warning, TEXT("ServerTeleportToPlaygroundIslandDock: No valid player start found for %s"), *GetName());
	}
}

void AFortPlayerControllerAthena::ServerStartMinigame()
{
	UE_LOG(LogFort, Log, TEXT("ServerStartMinigame"));
}

void AFortPlayerControllerAthena::ServerUpdateActorOptions(AActor* OptionsTarget, TArray<FString>& OptionsKeys, TArray<FString>& OptionsValues)
{
	UE_LOG(LogFort, Log, TEXT("ServerUpdateActorOptions: %s"), *OptionsTarget->GetName());

	if (OptionsTarget)
	{
		for (int i = 0; i < OptionsKeys.Num(); i++)
			UE_LOG(LogFort, Log, TEXT("\t -> %s : %s"), *OptionsKeys[i], *OptionsValues[i]);
	}
}

bool AFortPlayerControllerAthena::ServerLoadPlotForPortal_Validate(AFortAthenaCreativePortal* Portal, FString& PlotItemId)
{
	return Portal != NULL && !PlotItemId.IsEmpty();
}

void AFortPlayerControllerAthena::ServerLoadPlotForPortal(AFortAthenaCreativePortal* Portal, FString& PlotItemId)
{
	UE_LOG(LogFort, Log, TEXT("ServerLoadPlotForPortal: %s, %s"), *Portal->GetName(), *PlotItemId);

	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);
	auto PlotItem = CreativeModeProfile->FindPlotById(PlotItemId);

	UE_LOG(LogFort, Log, TEXT("PlotItem: %s"), *PlotItem->GetName());

	if (!PlotItem) return;

	if (UKismetTextLibrary::Conv_TextToString(Portal->IslandInfo.AltTitle) == 
		PlotItem->IslandTitle)
	{
		return;
	}

	if (auto PortalManager = GameState->CreativePortalManager)
	{
		PortalManager->LoadPlot(this, PlotItem);
	}
}

void AFortPlayerControllerAthena::Tick()
{
	if (!MyFortPawn || !WorldInventory)
		return;

	if (auto Weapon = MyFortPawn->CurrentWeapon)
	{
		WorldInventory->UpdateItem(Weapon->ItemEntryGuid, -1, Weapon->AmmoCount, Weapon);
	}

	if (FMath::IsNearlyZero(MyFortPawn->GetHealth()) && !MyFortPawn->bIsDBNO)
	{
		ServerSuicide();
	}
}

bool AFortPlayerControllerAthena::IsTeamDead()
{
	auto FortPlayerState = Cast<AFortPlayerStateAthena>(PlayerState);
	
	if (!FortPlayerState)
		return true;

	return !FortPlayerState->PlayerTeam->IsAlive();
}

TArray<TArray<TPair<FString, int>>> AFortPlayerControllerAthena::GetLoadout()
{
	if (UKismetSystemLibrary::GetFortniteVersion() == 9.41)
	{
		TArray<TArray<TPair<FString, int>>> Slots =
		{
			{ // Slot 1
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03", 1 }, // Green Pump
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03", 1 }, // Blue Pump
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03", 1 }, // Purple Pump
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03", 1 }, // Gold Pump
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Combat_Athena_R_Ore_T03.WID_Shotgun_Combat_Athena_R_Ore_T03", 1 }, // Blue Combat
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Combat_Athena_VR_Ore_T03.WID_Shotgun_Combat_Athena_VR_Ore_T03", 1 }, // Purple Combat
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Combat_Athena_SR_Ore_T03.WID_Shotgun_Combat_Athena_SR_Ore_T03", 1 }, // Gold Combat
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03", 1 }, // Blue Tac
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03", 1 }, // Purple Tac
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03", 1 } // Gold Tac
			},
			{ // Slot 2
				{ L"/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03", 1 }, // Purple Scar
				{ L"/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03", 1 }, // Gold Scar
				{ L"/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03", 1 }, // Blue AR
				{ L"/Game/Athena/Items/Weapons/WID_Assault_Suppressed_Athena_VR_Ore_T03.WID_Assault_Suppressed_Athena_VR_Ore_T03", 1 }, // Purple Suppressed AR
				{ L"/Game/Athena/Items/Weapons/WID_Assault_Suppressed_Athena_SR_Ore_T03.WID_Assault_Suppressed_Athena_SR_Ore_T03", 1 }, // Gold Suppressed AR
				{ L"/Game/Athena/Items/Weapons/WID_Assault_PistolCaliber_AR_Athena_R_Ore_T03.WID_Assault_PistolCaliber_AR_Athena_R_Ore_T03", 1 }, // Blue Tac AR
				{ L"/Game/Athena/Items/Weapons/WID_Assault_PistolCaliber_AR_Athena_VR_Ore_T03.WID_Assault_PistolCaliber_AR_Athena_VR_Ore_T03", 1 }, // Purple Tac AR
				{ L"/Game/Athena/Items/Weapons/WID_Assault_PistolCaliber_AR_Athena_SR_Ore_T03.WID_Assault_PistolCaliber_AR_Athena_SR_Ore_T03", 1 } // Gold Tac AR
			},
			{ // Slot 3
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_BurstFireSMG_Athena_R_Ore_T03.WID_Pistol_BurstFireSMG_Athena_R_Ore_T03", 1 }, // Blue Burst SMG
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_R_Ore_T03.WID_Pistol_Scavenger_Athena_R_Ore_T03", 1 }, // Blue Tac SMG
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_VR_Ore_T03.WID_Pistol_Scavenger_Athena_VR_Ore_T03", 1 }, // Purple Tac SMG
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03", 1 }, // Blue SMG
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03", 1 }, // Purple SMG
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03", 1 }, // Gold SMG
				{ L"/Game/Athena/Items/Weapons/WID_Assault_Infantry_Athena_VR.WID_Assault_Infantry_Athena_VR", 1 }, // Purple Infantry
				{ L"/Game/Athena/Items/Weapons/WID_Assault_Infantry_Athena_SR.WID_Assault_Infantry_Athena_SR", 1 }, // Gold Infantry
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_VR_Ore_T03.WID_Pistol_HandCannon_Athena_VR_Ore_T03", 1 }, // Purple Deagle
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_SR_Ore_T03.WID_Pistol_HandCannon_Athena_SR_Ore_T03", 1 }, // Gold Deagle
				{ L"/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03", 1 }, // Blue Bolt
				{ L"/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03", 1 }, // Purple Bolt
				{ L"/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03", 1 }, // Gold Bolt
				{ L"/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03", 1 }, // Purple Heavy
				{ L"/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_SR_Ore_T03.WID_Sniper_Heavy_Athena_SR_Ore_T03", 1 } // Gold Heavy
			},
			{ // Slot 4
				{ L"/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall", 6 }, // Mini Shields
				{ L"/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields", 3 }, // Shields
				{ L"/Game/Athena/Items/Consumables/PurpleStuff/Athena_PurpleStuff.Athena_PurpleStuff", 2 }, // Slurp Juice
				{ L"/Game/Athena/Items/Consumables/ChillBronco/Athena_ChillBronco.Athena_ChillBronco", 6 } // Chug Splash
			},
			{ // Slot 5
				{ L"/Game/Athena/Items/Weapons/LTM/WID_Hook_Gun_Slide.WID_Hook_Gun_Slide", 1 }, // Grappler
				{ L"/Game/Athena/Items/Consumables/RiftItem/Athena_Rift_Item.Athena_Rift_Item", 2 }, // Rift To Go
				{ L"/Game/Athena/Items/Consumables/TowerGrenade/Athena_TowerGrenade.Athena_TowerGrenade", 2 }, // Port A Fort
				{ L"/Game/Athena/Items/Consumables/Bandage/Athena_Bandage.Athena_Bandage", 15 }, // Bandages
				{ L"/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit", 3 }, // Medkits
				{ L"/Game/Athena/Items/Consumables/ChillBronco/Athena_ChillBronco.Athena_ChillBronco", 6 }, // Chug Splash
				{ L"/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall", 6 }, // Mini Shields
				{ L"/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields", 3 }, // Shields
				{ L"/Game/Athena/Items/Consumables/ShockwaveGrenade/Athena_ShockGrenade.Athena_ShockGrenade", 6 }, // Shockwaves
				{ L"/Game/Athena/Items/Consumables/DeployableStorm/Athena_DogSweater.Athena_DogSweater", 1 } // Storm Flips
			},
			{ // Traps
				{ L"/Game/Athena/Items/Traps/TID_Floor_Player_Launch_Pad_Athena.TID_Floor_Player_Launch_Pad_Athena", 1 }, // Launch Pad
				{ L"/Game/Athena/Items/Traps/TID_ContextTrap_Athena.TID_ContextTrap_Athena", 1 } // Damage Trap
			}
		};

		return Slots;
	}
	else
	{
		TArray<TArray<TPair<FString, int>>> Slots =
		{
			{ // Slot 1
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03", 1 }, // Green Pump
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03", 1 }, // Blue Pump
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03", 1 }, // Purple Pump
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03", 1 }, // Gold Pump
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03", 1 }, // Blue Tac
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03", 1 }, // Purple Tac
				{ L"/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03", 1 } // Gold Tac
			},
			{ // Slot 2
				{ L"/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03", 1 }, // Purple Scar
				{ L"/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03", 1 }, // Gold Scar
				{ L"/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03", 1 }, // Blue AR
				{ L"/Game/Athena/Items/Weapons/WID_Assault_Suppressed_Athena_VR_Ore_T03.WID_Assault_Suppressed_Athena_VR_Ore_T03", 1 }, // Purple Suppressed AR
				{ L"/Game/Athena/Items/Weapons/WID_Assault_Suppressed_Athena_SR_Ore_T03.WID_Assault_Suppressed_Athena_SR_Ore_T03", 1 } // Gold Suppressed AR
			},
			{ // Slot 3
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_BurstFireSMG_Athena_R_Ore_T03.WID_Pistol_BurstFireSMG_Athena_R_Ore_T03", 1 }, // Blue Burst SMG
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_R_Ore_T03.WID_Pistol_Scavenger_Athena_R_Ore_T03", 1 }, // Blue Tac SMG
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_VR_Ore_T03.WID_Pistol_Scavenger_Athena_VR_Ore_T03", 1 }, // Purple Tac SMG
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03", 1 }, // Blue SMG
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03", 1 }, // Purple SMG
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03", 1 }, // Gold SMG
				{ L"/Game/Athena/Items/Weapons/WID_Assault_Infantry_Athena_VR.WID_Assault_Infantry_Athena_VR", 1 }, // Purple Infantry
				{ L"/Game/Athena/Items/Weapons/WID_Assault_Infantry_Athena_SR.WID_Assault_Infantry_Athena_SR", 1 }, // Gold Infantry
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_VR_Ore_T03.WID_Pistol_HandCannon_Athena_VR_Ore_T03", 1 }, // Purple Deagle
				{ L"/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_SR_Ore_T03.WID_Pistol_HandCannon_Athena_SR_Ore_T03", 1 }, // Gold Deagle
				{ L"/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03", 1 }, // Blue Bolt
				{ L"/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03", 1 }, // Purple Bolt
				{ L"/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03", 1 }, // Gold Bolt
				{ L"/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03", 1 }, // Purple Heavy
				{ L"/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_SR_Ore_T03.WID_Sniper_Heavy_Athena_SR_Ore_T03", 1 } // Gold Heavy
			},
			{ // Slot 4
				{ L"/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall", 6 }, // Mini Shields
				{ L"/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields", 3 }, // Shields
				{ L"/Game/Athena/Items/Consumables/PurpleStuff/Athena_PurpleStuff.Athena_PurpleStuff", 2 }, // Slurp Juice
				{ L"/Game/Athena/Items/Consumables/ChillBronco/Athena_ChillBronco.Athena_ChillBronco", 6 } // Chug Splash
			},
			{ // Slot 5
				{ L"/Game/Athena/Items/Weapons/LTM/WID_Hook_Gun_Slide.WID_Hook_Gun_Slide", 1 }, // Grappler
				{ L"/Game/Athena/Items/Consumables/RiftItem/Athena_Rift_Item.Athena_Rift_Item", 2 }, // Rift To Go
				{ L"/Game/Athena/Items/Consumables/TowerGrenade/Athena_TowerGrenade.Athena_TowerGrenade", 2 }, // Port A Fort
				{ L"/Game/Athena/Items/Consumables/Bandage/Athena_Bandage.Athena_Bandage", 15 }, // Bandages
				{ L"/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit", 3 }, // Medkits
				{ L"/Game/Athena/Items/Consumables/ChillBronco/Athena_ChillBronco.Athena_ChillBronco", 6 }, // Chug Splash
				{ L"/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall", 6 }, // Mini Shields
				{ L"/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields", 3 }, // Shields
				{ L"/Game/Athena/Items/Consumables/ShockwaveGrenade/Athena_ShockGrenade.Athena_ShockGrenade", 6 }, // Shockwaves
				{ L"/Game/Athena/Items/Consumables/DeployableStorm/Athena_DogSweater.Athena_DogSweater", 1 }, // Storm Flips
				{ L"/Game/Athena/Items/Consumables/JollyRascal/WID_Athena_JollyRascal.WID_Athena_JollyRascal", 1} // Junk Rift
			},
			{ // Traps
				{ L"/Game/Athena/Items/Traps/TID_Floor_Player_Launch_Pad_Athena.TID_Floor_Player_Launch_Pad_Athena", 1 }, // Launch Pad
				{ L"/Game/Athena/Items/Traps/TID_ContextTrap_Athena.TID_ContextTrap_Athena", 1 } // Damage Trap
			}
		};

		return Slots;
	}
}

void AFortPlayerControllerAthena::EquipLoadout()
{
	for (auto& Slot : GetLoadout())
	{
		auto Item = Slot[UKismetMathLibrary::RandomIntegerInRange(0, Slot.Num() - 1)];
		auto ItemDef = LoadObject<UFortWeaponItemDefinition>(*Item.Key);

		if (!ItemDef) continue;

		bool NeedsUpdate;
		WorldInventory->AddItem(ItemDef, Item.Value, NeedsUpdate, UFortKismetLibrary::GetClipSize(ItemDef));
	}

	WorldInventory->AddItem(LoadObject<UFortItemDefinition>(L"/Game/Items/ResourcePickups/WoodItemData.WoodItemData"), 500);
	WorldInventory->AddItem(LoadObject<UFortItemDefinition>(L"/Game/Items/ResourcePickups/StoneItemData.StoneItemData"), 500);
	WorldInventory->AddItem(LoadObject<UFortItemDefinition>(L"/Game/Items/ResourcePickups/MetalItemData.MetalItemData"), 500);

	WorldInventory->AddItem(LoadObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells"), 35);
	WorldInventory->AddItem(LoadObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium"), 150);
	WorldInventory->AddItem(LoadObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight"), 150);
	WorldInventory->AddItem(LoadObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy"), 20);
	WorldInventory->AddItem(LoadObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets"), 2);

	WorldInventory->Update();
}

TArray<TWeakObjectPtr<AFortPlayerStateAthena>>* AFortPlayerControllerAthena::FindTeamMembers()
{
	auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState);
	auto FortPlayerState = Cast<AFortPlayerStateAthena>(PlayerState);

	if (GameState && FortPlayerState)
		return GameState->FindSquadMembers(FortPlayerState->SquadId);

	return NULL;
}

void AFortPlayerControllerAthena::HandleCreativeProfileUpdate()
{
	UE_LOG(LogFort, Log, TEXT("Creative Profile Updated"));

	TArray<int> DeletedCreativeIslands;

	for (int i = 0; i < CreativeIslands.Num(); i++)
	{
		bool Found = false;

		for (auto& Item : CreativeModeProfile->State->ItemsContainer.Items)
		{
			if (Item.Value->InstanceId == CreativeIslands[i].McpId)
			{
				Found = true;
				break;
			}
		}

		if (!Found)
		{
			DeletedCreativeIslands.Add(i);
		}
	}

	for (auto& DeletedCreativeIsland : DeletedCreativeIslands)
	{
		UKismetArrayLibrary::Array_Remove(CreativeIslands, this->GetClassProperty(L"CreativeIslands"), DeletedCreativeIsland);
	}

	for (auto Item : CreativeModeProfile->State->ItemsContainer.Items)
	{
		auto McpItem = Item.Value.Get();
		auto CreativeIsland = CreativeIslands.FindByPredicate([McpItem](FCreativeIslandData& IslandData)
			{ return IslandData.McpId == McpItem->InstanceId; });

		bool IsNewEntry = false;

		if (!CreativeIsland)
		{
			CreativeIsland = FCreativeIslandData::Allocate();
			IsNewEntry = true;
		}

		auto PlotInstance = Cast<UFortCreativeRealEstatePlotItem>(McpItem->Instance);

		if (!PlotInstance)
		{
			FString PlotId;
			McpItem->TemplateId.Split(L":", NULL, &PlotId);

			auto PlotItemDefinition = FindObject<UFortCreativeRealEstatePlotItemDefinition>(ANY_PACKAGE, *PlotId);

			if (!PlotItemDefinition)
				continue;

			PlotInstance = PlotItemDefinition->Create<UFortCreativeRealEstatePlotItem>();
			PlotInstance->InstanceID = McpItem->InstanceId;
			McpItem->Instance = PlotInstance;
		}

		PlotInstance->ParseAttributes(McpItem);

		CreativeIsland->McpId = McpItem->InstanceId;
		CreativeIsland->IslandName = UKismetTextLibrary::Conv_StringToText(PlotInstance->IslandTitle);
		CreativeIsland->LastLoadedDate = PlotInstance->LastUsedDate;
		CreativeIsland->DeletedAt = PlotInstance->DeletedAt;
		CreativeIsland->bIsDeleted = false;
		
		if (IsNewEntry)
		{
			UKismetArrayLibrary::Array_Add(CreativeIslands,
				this->GetClassProperty(L"CreativeIslands"), *CreativeIsland);
		}
	}

	MaxPlotCount = 50;
	OnRep_CreativeIslands();

	if (OwnedPortal && !OwnedPortal->bPortalOpen && OwnedPortal->LinkedVolume)
	{
		if (OwnedPortal->LinkedVolume->VolumeState == EVolumeState::Initializing)
			return;

		if (auto GameState = Cast<AFortGameStateAthena>(GetWorld()->GameState))
		{
			if (auto PortalManager = GameState->CreativePortalManager)
			{
				FCreativeIslandData* MostRecentIsland = nullptr;
				FDateTime LatestDate;

				for (auto& Island : CreativeIslands)
				{
					if (!MostRecentIsland || Island.LastLoadedDate > LatestDate)
					{
						MostRecentIsland = &Island;
						LatestDate = Island.LastLoadedDate;
					}
				}

				if (!MostRecentIsland)
				{
					if (CreativeIslands.Num())
					{
						MostRecentIsland = &CreativeIslands[0];
					}
					else
					{
						auto PlotItemDefinition = LoadObject<UFortCreativeRealEstatePlotItemDefinition>(L"/Game/Playgrounds/Items/Plots/FlatGrass_Large.FlatGrass_Large");
						auto PlotInstance = PlotItemDefinition->Create<UFortCreativeRealEstatePlotItem>();

						PortalManager->LoadPlot(this, PlotInstance);
						return;
					}
				}

				auto PlotInstance = CreativeModeProfile->FindPlotById(MostRecentIsland->McpId);

				if (!PlotInstance)
					return;

				PortalManager->LoadPlot(this, PlotInstance);
			}
		}
	}
}