#pragma once
#include "FortAthenaSKVehicle.h"
#include "Components/PrimitiveComponent.h"
#include "Projectiles/FortOctopusTowhookAttachableProjectile.h"

class FNetTowhookAttachState
{
	GENERATED_USTRUCT_BODY(FNetTowhookAttachState, FortniteGame)

	unsigned char Pad[0x20];

public:
	
	UPROPERTY_STRUCT(UPrimitiveComponent*, Component)
	UPrimitiveComponent* Component;

	UPROPERTY_STRUCT(FVector, LocalLocation)
	FVector LocalLocation;

	UPROPERTY_STRUCT(FVector, LocalNormal)
	FVector LocalNormal;
};

class AFortOctopusVehicle : public AFortAthenaSKVehicle
{
	GENERATED_UCLASS_BODY(AFortOctopusVehicle, FortniteGame)
public:

	UPROPERTY(TWeakObjectPtr<AFortOctopusTowhookAttachableProjectile>, TowHookProjectile)
	TWeakObjectPtr<AFortOctopusTowhookAttachableProjectile> TowHookProjectile;

	UPROPERTY(TWeakObjectPtr<UPrimitiveComponent>, TowhookAttachedComponent)
	TWeakObjectPtr<UPrimitiveComponent> TowhookAttachedComponent;

	UPROPERTY(FVector, NetTowhookAimDir)
	FVector NetTowhookAimDir;

	UPROPERTY(FNetTowhookAttachState, ReplicatedAttachState)
	FNetTowhookAttachState ReplicatedAttachState;

	UPROPERTY(FNetTowhookAttachState, LocalAttachState)
	FNetTowhookAttachState LocalAttachState;
	
	void ServerUpdateTowhook(FVector InNetTowhookAimDir);
	void ServerSetTowhookAttachState(FNetTowhookAttachState InTowhookAttachState);
	void SetTowhookAttachState(UPrimitiveComponent* Component, FVector WorldLocation, FVector WorldNormal);

	void UpdateAimTrace(FVector TowhookAimDir, AFortPlayerController* PlayerController);

	void OnRep_ReplicatedAttachState();
	void OnRep_NetTowhookAimDir();

	void DestroyTowhookProjectile();
	void BreakTowhook();
};