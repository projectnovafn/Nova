#pragma once
#include "Building/BuildingGameplayActor.h"
#include "ItemDefinitions/FortPlaysetItemDefinition.h"

enum class EVolumeState : uint8
{
	Uninitialized = 0,
	ReadOnly = 1,
	Initializing = 2,
	Ready = 3
};

class AFortVolume : public ABuildingGameplayActor
{
	GENERATED_UCLASS_BODY(AFortVolume, FortniteGame)
public:

	UPROPERTY(UFortPlaysetItemDefinition*, CurrentPlayset)
	UFortPlaysetItemDefinition* CurrentPlayset;

	UPROPERTY_NOTIFY(EVolumeState, VolumeState)
	EVolumeState VolumeState;

	void ServerClearVolume();

	TArray<AActor*> GetActorsWithinVolumeByClass(UClass* ActorClass);

	template <typename T>
	TArray<T*> GetActorsWithinVolumeByClass()
	{
		auto Actors = GetActorsWithinVolumeByClass(T::StaticClass());
		return *(TArray<T*>*) & Actors;
	}
};