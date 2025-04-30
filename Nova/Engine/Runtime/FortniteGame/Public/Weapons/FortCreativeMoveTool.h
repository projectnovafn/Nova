#pragma once
#include "FortWeapon.h"

class UObjectInteractionBehavior;

class FOriginalAndSpawnedPair
{
public:
	AActor* OriginalActor;
	AActor* SpawnedActor;
};

class FCreativeSelectedActorInfo
{
public:
	AActor* Actor;
private:
	unsigned char UnknownData00[0x8];

public:
	FTransform ActorToSelectionAtDragStart;
	FVector DragStartGridSnapPoint;
	float OriginalRelevancyDistance;
	bool bWasCollisionEnabled;
	bool bWasDormant;

private:
	unsigned char UnknownData01[0xE];
};

class AFortCreativeMoveTool : public AFortWeapon
{
	GENERATED_UCLASS_BODY(AFortCreativeMoveTool, FortniteGame)
public:

	UPROPERTY(UObjectInteractionBehavior*, ActiveMovementMode)
	UObjectInteractionBehavior* ActiveMovementMode;

	UPROPERTY(TArray<UObjectInteractionBehavior*>, InteractionBehaviors)
	TArray<UObjectInteractionBehavior*> InteractionBehaviors;

	UPROPERTY_NOTIFY(TArray<FOriginalAndSpawnedPair>, NewlyPlacedActors)
	TArray<FOriginalAndSpawnedPair> NewlyPlacedActors;

	UPROPERTY(TArray<FCreativeSelectedActorInfo>, SelectedActors)
	TArray<FCreativeSelectedActorInfo> SelectedActors;

	UPROPERTY_BOOL(bIsPreviewingMove)
	bool bIsPreviewingMove;

	UPROPERTY_BOOL(bClientNeedsToProcessNewlyPlacedActors)
	bool bClientNeedsToProcessNewlyPlacedActors;

	UPROPERTY_BOOL(bShouldDestroyPropsWhenPlacing)
	bool bShouldDestroyPropsWhenPlacing;

	void ComputeSelectionSetTransformAndBounds(FTransform& OutTransform, FBox& OutBounds);

	void ServerStartInteracting(TArray<AActor*>& Actors, FTransform DragStart);
	void ServerDuplicateStartInteracting(TArray<AActor*>& Actors, FTransform DragStart);

	void ServerSpawnActorWithTransform(AActor* ActorToSpawn, FTransform& TargetTransform, bool bAllowOverlap, bool bAllowGravity, bool bIgnoreStructuralIssues, bool bForPreviewing);

	void ClientStopInteracting();
	void ClientStartInteracting(UObjectInteractionBehavior* NewActiveMovementMode, TArray<FCreativeSelectedActorInfo> NewSelectedActors, FTransform NewSelectionToWorld, FBox NewSelectionSpaceActorBounds);
};