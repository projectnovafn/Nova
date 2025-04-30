#include "Weapons/FortCreativeMoveTool.h"

#include "Athena/FortGameStateAthena.h"

#include "Components/ObjectInteractionBehavior.h"

#include "Building/BuildingActor.h"
#include "Building/BuildingStructuralSupportSystem.h"

void AFortCreativeMoveTool::ComputeSelectionSetTransformAndBounds(FTransform& OutTransform, FBox& OutBounds)
{
	if (!SelectedActors.Num())
		return;

	auto SelectedActor = SelectedActors[0].Actor;

	if (SelectedActor)
	{
		OutTransform = SelectedActor->GetTransform();
	}
}

void AFortCreativeMoveTool::ServerStartInteracting(TArray<AActor*>& Actors, FTransform DragStart)
{
	UE_LOG(LogFort, Log, TEXT("ServerStartInteracting: %s"), *ActiveMovementMode->GetName());

	SelectedActors.Empty();
	NewlyPlacedActors.Empty();

	if (!ActiveMovementMode)
	{
		ActiveMovementMode = InteractionBehaviors[0];
	}

	for (auto& InteractionBehavior : InteractionBehaviors)
	{
		UE_LOG(LogFort, Log, TEXT("InteractionBehavior: %s"), *InteractionBehavior->GetName());
	}
	
	for (auto& Actor : Actors)
	{
		FCreativeSelectedActorInfo SelectedActorInfo{};
		SelectedActorInfo.Actor = Actor;
		SelectedActorInfo.ActorToSelectionAtDragStart = DragStart;

		SelectedActors.Add(SelectedActorInfo);
	}

	FTransform Transform;
	FBox Bounds;

	ComputeSelectionSetTransformAndBounds(Transform, Bounds);
	ClientStartInteracting(ActiveMovementMode, SelectedActors, Transform, Bounds);

	ActiveMovementMode->StartCreativeInteractionOnServer();
}

void AFortCreativeMoveTool::ServerDuplicateStartInteracting(TArray<AActor*>& Actors, FTransform DragStart)
{
	UE_LOG(LogFort, Log, TEXT("ServerDuplicateStartInteracting: %s, %i"), *GetName(), Actors.Num());

	TArray<AActor*> InteractionActors;

	for (auto& Actor : Actors)
	{
		UE_LOG(LogFort, Log, TEXT("Actor: %s"), *Actor->GetName());

		if (Actor->IsA<ABuildingActor>())
		{
			auto SpawnedActor = GetWorld()->SpawnActor<ABuildingActor>(Actor->GetTransform(), Actor->Class);

			if (!SpawnedActor)
				return;

			SpawnedActor->InitializeKismetSpawnedBuildingActor(SpawnedActor, NULL, false);

			InteractionActors.Add(SpawnedActor);
		}
	}

	ServerStartInteracting(InteractionActors, DragStart);
}

void AFortCreativeMoveTool::ServerSpawnActorWithTransform(AActor* ActorToSpawn, FTransform& TargetTransform, bool bAllowOverlap, bool bAllowGravity, bool bIgnoreStructuralIssues, bool bForPreviewing)
{
	UE_LOG(LogFort, Log, TEXT("ServerSpawnActorWithTransform: %s"), *ActorToSpawn->GetName());

	for (auto& NewlyPlacedActor : NewlyPlacedActors)
	{
		if (NewlyPlacedActor.OriginalActor == ActorToSpawn)
			return;
	}

	if (ActorToSpawn->IsA<ABuildingActor>())
	{
		auto SpawnedActor = GetWorld()->SpawnActor<ABuildingActor>(TargetTransform, ActorToSpawn->Class);

		if (!SpawnedActor)
			return;

		SpawnedActor->InitializeKismetSpawnedBuildingActor(SpawnedActor, NULL, false);
		
		bClientNeedsToProcessNewlyPlacedActors = true;

		NewlyPlacedActors.Add(FOriginalAndSpawnedPair{ ActorToSpawn, SpawnedActor });
		OnRep_NewlyPlacedActors();
	}
}

void AFortCreativeMoveTool::ClientStopInteracting()
{
	this->ProcessEvent(L"ClientStopInteracting");
}

void AFortCreativeMoveTool::ClientStartInteracting(UObjectInteractionBehavior* NewActiveMovementMode, TArray<FCreativeSelectedActorInfo> NewSelectedActors, FTransform NewSelectionToWorld, FBox NewSelectionSpaceActorBounds)
{
	struct
	{
		UObjectInteractionBehavior* NewActiveMovementMode;
		TArray<FCreativeSelectedActorInfo> NewSelectedActors;
		FTransform NewSelectionToWorld;
		FBox NewSelectionSpaceActorBounds;
	} params(NewActiveMovementMode, NewSelectedActors, NewSelectionToWorld, NewSelectionSpaceActorBounds);

	this->ProcessEvent(L"ClientStartInteracting", &params);
}