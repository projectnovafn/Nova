#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "EngineTypes.h"

#include "GameFramework/Actor.h"
#include "GameInstance.h"

#include "AI/NavigationSystemBase.h"

class UNetDriver;
class AGameStateBase;
class AGameModeBase;

struct FLevelCollection
{
	UNetDriver* GetNetDriver() const { return NetDriver; }

	void SetNetDriver(UNetDriver* const InNetDriver) { NetDriver = InNetDriver; }

private:

	ELevelCollectionType CollectionType;
	UObject* GameState;
	UNetDriver* NetDriver;
};

class FNetworkNotify
{
	
};

class UWorld final : public UObject, public FNetworkNotify
{
	GENERATED_UCLASS_BODY(UWorld, Engine)
public:

	UPROPERTY(UNetDriver*, NetDriver)
	UNetDriver* NetDriver;

	UPROPERTY(AGameStateBase*, GameState)
	AGameStateBase* GameState;

	UPROPERTY(AGameModeBase*, AuthorityGameMode)
	AGameModeBase* AuthorityGameMode;

	UPROPERTY(UGameInstance*, OwningGameInstance)
	UGameInstance* GameInstance;

	UPROPERTY(UNavigationSystemBase*, NavigationSystem)
	UNavigationSystemBase* NavigationSystem;

	FLevelCollection* FindCollectionByType(const ELevelCollectionType InType);

	bool Listen(FURL& InURL);

	ENetMode GetNetMode();

	void Tick(ELevelTick TickType, float DeltaSeconds);

	AActor* SpawnActor(FVector Location, FRotator Rotation, UClass* Class);
	AActor* SpawnActor(FTransform Transform, UClass* Class);

	template <typename T>
	T* SpawnActor(FVector Location, FRotator Rotation = FRotator(0, 0, 0), UClass* Class = T::StaticClass())
	{
		return (T*)SpawnActor(Location, Rotation, Class);
	}

	template <typename T>
	T* SpawnActor(FTransform Transform, UClass* Class = T::StaticClass())
	{
		return (T*)SpawnActor(Transform, Class);
	}
};

inline UWorld* GWorld = NULL;