#pragma once

#include "CoreUObject.h"
#include "Engine/EngineTypes.h"
#include "Engine/EngineBaseTypes.h"
#include "Logging/LogMacros.h"
#include "Components/SceneComponent.h"

class UWorld;
class ULevel;

class AActor : public UObject
{
	GENERATED_UCLASS_BODY(AActor, Engine)
private:

	UPROPERTY(AActor*, Owner)
	AActor* Owner;

	UPROPERTY(ENetRole, Role)
	ENetRole Role;

public:

	UPROPERTY(TArray<AActor*>, Children)
	TArray<AActor*> Children;

	AActor* GetOwner()
	{
		return Owner;
	}

	void SetOwner(AActor* NewOwner)
	{
		Owner = NewOwner;
	}

	ENetMode GetNetMode();

	ULevel* GetLevel();
	UWorld* GetWorld();

	FVector GetActorLocation();
	FRotator GetActorRotation();
	
	FVector GetActorRightVector();
	FVector GetActorForwardVector();

	FTransform GetTransform();

	USceneComponent* GetRootComponent();

	TArray<UActorComponent*> GetComponentsByClass(UClass* ComponentClass);

	template <typename T>
	TArray<T*> GetComponents()
	{
		auto Components = GetComponentsByClass(T::StaticClass());
		return *(TArray<T*>*)&Components;
	}

	template <typename T>
	T* GetComponent()
	{
		auto Components = GetComponentsByClass(T::StaticClass());
		return Components.Num() ? (T*)Components[0] : NULL;
	}

	float GetDistanceTo(AActor* OtherActor);
	float GetDistanceTo2D(AActor* OtherActor);

	void SetActorLocation(FVector NewLocation, bool bSweep = false, bool bTeleport = true);
	void SetActorRotation(FRotator NewRotation, bool bSweep = false, bool bTeleport = true);

	bool TeleportTo(FVector DestLocation, FRotator DestRotation);

	void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation);

	void DestroyActor();

	void SetReplicateMovement(bool bReplicateMovement);

	void OnRep_ReplicateMovement();

	bool HasAuthority() { return (Role == ROLE_Authority); }
};