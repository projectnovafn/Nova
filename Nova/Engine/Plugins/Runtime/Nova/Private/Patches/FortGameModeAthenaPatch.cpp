#include "Patches/FortGameModeAthenaPatch.h"
#include "KismetDetoursLibrary.h"

#include "FortGameModeEmptyDedicated.h"

#include "Nova.h"

static bool (*_ReadyToStartMatch)(AFortGameModeAthena*);
static bool (*_ReadyToEndMatch)(AFortGameModeAthena*);
static APawn* (*_SpawnDefaultPawnFor)(AFortGameModeAthena*, AController*, AActor*);
static void (*_OnSafeZonePhaseChanged)(AFortGameModeAthena*, int);
static bool (*_PlayerCanRestart)(AFortGameModeAthena*, APlayerController*);
static bool (*_GenerateVoiceChatToken)(AFortGameModeAthena*, AFortPlayerControllerZone*, EVoiceChannelType, FString const&, FString&, FString&);

UDetour* OnSafeZonePhaseChangedDetour = NULL;
UDetour* GenerateVoiceChatTokenDetour = NULL;

bool AFortGameModeAthenaPatch::ReadyToStartMatchHook(AFortGameModeAthena* GameMode)
{
	GameMode->InitGame();

	if (GameMode->ReadyToStartMatch() || _ReadyToStartMatch(GameMode))
	{
		GameMode->HandleMatchHasStarted();

		return true;
	}

	return false;
}

bool AFortGameModeAthenaPatch::ReadyToEndMatchHook(AFortGameModeAthena* GameMode)
{
	return GameMode->ReadyToEndMatch() || _ReadyToEndMatch(GameMode);
}

APawn* AFortGameModeAthenaPatch::SpawnDefaultPawnForHook(AFortGameModeAthena* GameMode, AController* NewPlayer, AActor* StartSpot)
{
	return GameMode->SpawnDefaultPawnFor(NewPlayer, StartSpot);
}

UClass** AFortGameModeAthenaPatch::GetGameSessionClassHook(AFortGameModeAthena* GameMode, UClass** Result)
{
	*Result = GameMode->GetGameSessionClass();
	return Result;
}

void AFortGameModeAthenaPatch::OnSafeZonePhaseChangedHook(AFortGameModeAthena* GameMode, int Phase)
{
	GameMode->OnSafeZonePhaseChanged(Phase);

	return _OnSafeZonePhaseChanged(GameMode, GNova->bIsLateGame ? (GNova->bHasSetZonePhase ? GameMode->SafeZonePhase + 1 : (GNova->bHasSetZonePhase = true, GameMode->GetLateSafeZoneIndex())) : Phase);
}

bool AFortGameModeAthenaPatch::PlayerCanRestartHook(AFortGameModeAthena* GameMode, APlayerController* Player)
{
	return GameMode->PlayerCanRestart(Player, _PlayerCanRestart);
}

bool AFortGameModeAthenaPatch::GenerateVoiceChatTokenHook(AFortGameModeAthena* GameMode, AFortPlayerControllerZone* PlayerController, EVoiceChannelType ChannelType, FString const& ChannelName, FString& Token, FString& Error)
{
	return GameMode->GenerateVoiceChatToken(PlayerController, ChannelType, ChannelName, Token, Error);
}

void AFortGameModeAthenaPatch::Init()
{
	_OnSafeZonePhaseChanged = UKismetMemoryLibrary::Get<decltype(_OnSafeZonePhaseChanged)>(L"AFortGameModeAthena::OnSafeZonePhaseChanged");
	_GenerateVoiceChatToken = UKismetMemoryLibrary::Get<decltype(_GenerateVoiceChatToken)>(L"AFortGameModeZone::GenerateVoiceChatToken");

	AFortGameModeAthena::StaticClass()->GetDefaultObject<UObject>()->VTable[UKismetMemoryLibrary::Get(L"AGameMode::GetGameSessionClass")] 
		= GetGameSessionClassHook;

	AFortGameModeEmptyDedicated::StaticClass()->GetDefaultObject<UObject>()->VTable[UKismetMemoryLibrary::Get(L"AGameMode::GetGameSessionClass")]
		= GetGameSessionClassHook;

	OnSafeZonePhaseChangedDetour = new UDetour();
	OnSafeZonePhaseChangedDetour->Init(_OnSafeZonePhaseChanged, OnSafeZonePhaseChangedHook);
	OnSafeZonePhaseChangedDetour->Commit();

	GenerateVoiceChatTokenDetour = new UDetour();
	GenerateVoiceChatTokenDetour->Init(_GenerateVoiceChatToken, GenerateVoiceChatTokenHook);
	GenerateVoiceChatTokenDetour->Commit();

	UKismetDetoursLibrary::AddReflectedDetour<AFortGameModeAthena>(
		L"ReadyToStartMatch", 
		EReflectedDetourType::VFSwap, 
		ReadyToStartMatchHook, 
		_ReadyToStartMatch
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortGameModeAthena>(
		L"ReadyToEndMatch",
		EReflectedDetourType::VFSwap,
		ReadyToEndMatchHook,
		_ReadyToEndMatch
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortGameModeAthena>(
		L"SpawnDefaultPawnFor",
		EReflectedDetourType::VFSwap,
		SpawnDefaultPawnForHook,
		_SpawnDefaultPawnFor
	);

	UKismetDetoursLibrary::AddReflectedDetour<AFortGameModeAthena>(
		L"PlayerCanRestart",
		EReflectedDetourType::CallDetour,
		PlayerCanRestartHook,
		_PlayerCanRestart,
		0,
		{ 0x48, 0x89, 0x5C }
	);
}