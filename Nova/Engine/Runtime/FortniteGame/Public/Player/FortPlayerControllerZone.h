#pragma once
#include "FortPlayerControllerGameplay.h"

class AFortPlayerState;
class AFortPawn;

enum class EVoiceChannelType
{
	Party = 0,
	GameServer = 1
};

class FFortPlayerDeathReport
{
	GENERATED_USTRUCT_BODY(FFortPlayerDeathReport, FortniteGame)

public:

	UPROPERTY_STRUCT(float, ServerTimeForRespawn)
	float ServerTimeForRespawn;

	UPROPERTY_STRUCT(float, ServerTimeForResurrect)
	float ServerTimeForResurrect;

	UPROPERTY_STRUCT(float, LethalDamage)
	float LethalDamage;

	UPROPERTY_STRUCT(AFortPlayerState*, KillerPlayerState)
	AFortPlayerState* KillerPlayerState;

	UPROPERTY_STRUCT(AFortPawn*, KillerPawn)
	AFortPawn* KillerPawn;

	UPROPERTY_STRUCT(AActor*, DamageCauser)
	AActor* DamageCauser;

	UPROPERTY_STRUCT_BOOL(bDroppedBackpack)
	bool bDroppedBackpack;

	UPROPERTY_STRUCT_BOOL(bNotifyUI)
	bool bNotifyUI;

	UPROPERTY_STRUCT(FGameplayTagContainer, Tags)
	FGameplayTagContainer Tags;
};

class AFortPlayerControllerZone : public AFortPlayerControllerGameplay
{
	GENERATED_UCLASS_BODY(AFortPlayerControllerZone, FortniteGame)
public:

	UPROPERTY(APawn*, PlayerToSpectateOnDeath)
	APawn* PlayerToSpectateOnDeath;

	UPROPERTY(FFortPlayerDeathReport, PlayerDeathReport)
	FFortPlayerDeathReport PlayerDeathReport;

	void SpectateOnDeath();
	void ClientOnPawnRevived(AController* EventInstigator);
	void ClientOnPawnSpawned();
	void ClientClearDeathNotification();
};