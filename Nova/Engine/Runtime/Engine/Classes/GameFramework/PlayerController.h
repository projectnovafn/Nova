#pragma once
#include "Controller.h"
#include "Pawn.h"

class APlayerCameraManager;
class UNetConnection;

class APlayerController : public AController
{
	GENERATED_UCLASS_BODY(APlayerController, Engine)
public:

	UPROPERTY(FVector, LastSpectatorSyncLocation)
	FVector LastSpectatorSyncLocation;

	UPROPERTY(FRotator, LastSpectatorSyncRotation)
	FRotator LastSpectatorSyncRotation;

	UPROPERTY(FName, StateName)
	FName StateName;

	UPROPERTY(APawn*, AcknowledgedPawn)
	APawn* AcknowledgedPawn;

	UPROPERTY(APlayerCameraManager*, PlayerCameraManager)
	APlayerCameraManager* PlayerCameraManager;

	UPROPERTY(UNetConnection*, NetConnection)
	UNetConnection* NetConnection;
};