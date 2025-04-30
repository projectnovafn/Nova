#pragma once
#include "Building/BuildingGameplayActor.h"
#include "Components/FortLevelSaveComponent.h"
#include "FortVolume.h"

class AFortPlayerControllerAthena;
class AFortPlayerPawnAthena;
class AFortPlayerPawn;

class FCreativeIslandInfo
{
	GENERATED_USTRUCT_BODY(FCreativeIslandInfo, FortniteGame)
public:

	UPROPERTY_STRUCT(FString, IslandTitle)
	FString IslandTitle;

	UPROPERTY_STRUCT(FString, IslandIntroduction)
	FString IslandIntroduction;

	UPROPERTY_STRUCT(FString, UserLocale)
	FString UserLocale;
};

class AFortAthenaCreativePortal : public ABuildingGameplayActor
{
	GENERATED_UCLASS_BODY(AFortAthenaCreativePortal, FortniteGame)
public:

	UPROPERTY(int, PortalIndex)
	int PortalIndex;

	UPROPERTY(AActor*, DestinationActor)
	AActor* DestinationActor;

	UPROPERTY(FText, LoadingText)
	FText LoadingText;

	UPROPERTY(FText, SavingText)
	FText SavingText;

	UPROPERTY(AFortVolume*, LinkedVolume)
	AFortVolume* LinkedVolume;

	UPROPERTY_NOTIFY(uint8, CurrentPopulation)
	uint8 CurrentPopulation;

	UPROPERTY_PTR(FUniqueNetIdRepl, OwningPlayer)
	FUniqueNetIdRepl* OwningPlayer;

	UPROPERTY(TArray<FUniqueNetIdRepl>, PlayersReady)
	TArray<FUniqueNetIdRepl> PlayersReady;

	UPROPERTY(FVector, TeleportLocation)
	FVector TeleportLocation;

	UPROPERTY_NOTIFY(FCreativeLoadedLinkData, IslandInfo)
	FCreativeLoadedLinkData IslandInfo;

	UPROPERTY_BOOL(bPortalOpen)
	bool bPortalOpen;

	void TeleportPlayerToLinkedVolume(AFortPlayerPawnAthena* Pawn, bool bUseSpawnTags);
	void OnPlayerPawnTeleported(AFortPlayerPawn* PlayerPawn, bool bTeleportedToIslandStart);

	void OnRep_PortalOpen();

	void Setup();
};