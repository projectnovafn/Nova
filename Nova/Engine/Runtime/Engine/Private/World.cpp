#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/NetDriver.h"
#include "Engine/GameplayStatics.h"

#include "Nova.h"
#include "FortGlobals.h"

#include "Athena/FortGameModeAthena.h"

FLevelCollection* UWorld::FindCollectionByType(const ELevelCollectionType InType)
{
	static auto FindCollectionByType = UKismetMemoryLibrary::Get<FLevelCollection* (*)(UWorld*, ELevelCollectionType)>(L"UWorld::FindCollectionByType");

	return FindCollectionByType(this, InType);
}

AActor* UWorld::SpawnActor(FVector Location, FRotator Rotation, UClass* Class)
{
	FTransform Transform(Rotation.Quaternion(), Location, FVector(1, 1, 1));

	return SpawnActor(Transform, Class);
}

AActor* UWorld::SpawnActor(FTransform Transform, UClass* Class)
{
	return UGameplayStatics::FinishSpawningActor(UGameplayStatics::BeginSpawningActorFromClass(this, Class, Transform), Transform);
}

bool UWorld::Listen(FURL& InURL)
{
	UE_LOG(LogWorld, Log, TEXT("UWorld::Listen"));

	if (NetDriver)
	{
		return false;
	}

	if (GEngine->CreateNamedNetDriver(this, NAME_GameNetDriver, NAME_GameNetDriver))
	{
		NetDriver = GEngine->FindNamedNetDriver(this, NAME_GameNetDriver);
		NetDriver->SetWorld(this);

		FLevelCollection* const SourceCollection = FindCollectionByType(ELevelCollectionType::DynamicSourceLevels);
		if (SourceCollection)
		{
			SourceCollection->SetNetDriver(NetDriver);
		}
		FLevelCollection* const StaticCollection = FindCollectionByType(ELevelCollectionType::StaticLevels);
		if (StaticCollection)
		{
			StaticCollection->SetNetDriver(NetDriver);
		}
	}

	if (NetDriver == NULL)
	{
		return false;
	}

	FString Error;
	if (!NetDriver->InitListen(this, InURL, false, Error))
	{
		NetDriver->SetWorld(NULL);
		NetDriver = NULL;

		FLevelCollection* SourceCollection = FindCollectionByType(ELevelCollectionType::DynamicSourceLevels);
		if (SourceCollection)
		{
			SourceCollection->SetNetDriver(nullptr);
		}

		FLevelCollection* StaticCollection = FindCollectionByType(ELevelCollectionType::StaticLevels);
		if (StaticCollection)
		{
			StaticCollection->SetNetDriver(nullptr);
		}

		return false;
	}

	return true;
}

ENetMode UWorld::GetNetMode()
{
	return NetDriver ? NM_DedicatedServer : NM_Standalone;
}

void UWorld::Tick(ELevelTick TickType, float DeltaSeconds)
{
	if (auto GameMode = Cast<AFortGameModeAthena>(AuthorityGameMode))
		GameMode->Tick();

	RunTaskQueue(ENamedThreads::GameThread);
}