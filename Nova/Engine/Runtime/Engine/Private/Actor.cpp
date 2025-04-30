#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/Level.h"

ENetMode AActor::GetNetMode()
{
	if (GetWorld() && GetWorld()->NetDriver)
		return NM_DedicatedServer;

	return NM_Standalone;
}

ULevel* AActor::GetLevel()
{
	return GetTypedOuter<ULevel>();
}

UWorld* AActor::GetWorld()
{
	if (auto Level = GetLevel())
	{
		return Level->OwningWorld;
	}
	
	return NULL;
}

FVector AActor::GetActorLocation()
{
	FVector ReturnValue;
	this->ProcessEvent(L"K2_GetActorLocation", &ReturnValue);

	return ReturnValue;
}

FRotator AActor::GetActorRotation()
{
	FRotator ReturnValue;
	this->ProcessEvent(L"K2_GetActorRotation", &ReturnValue);

	return ReturnValue;
}

FVector AActor::GetActorRightVector()
{
	FVector ReturnValue;
	this->ProcessEvent(L"GetActorRightVector", &ReturnValue);

	return ReturnValue;
}

FVector AActor::GetActorForwardVector()
{
	FVector ReturnValue;
	this->ProcessEvent(L"GetActorForwardVector", &ReturnValue);

	return ReturnValue;
}

FTransform AActor::GetTransform()
{
	FTransform ReturnValue;
	this->ProcessEvent(L"GetTransform", &ReturnValue);

	return ReturnValue;
}

USceneComponent* AActor::GetRootComponent()
{
	USceneComponent* ReturnValue;
	this->ProcessEvent(L"K2_GetRootComponent", &ReturnValue);

	return ReturnValue;
}

TArray<UActorComponent*> AActor::GetComponentsByClass(UClass* ComponentClass)
{
	struct
	{
		UClass* ComponentClass;
		TArray<UActorComponent*> ReturnValue;
	} params(ComponentClass);

	this->ProcessEvent(L"GetComponentsByClass", &params);

	return params.ReturnValue;
}

float AActor::GetDistanceTo(AActor* OtherActor)
{
	struct
	{
		AActor* OtherActor;
		float ReturnValue;
	} params(OtherActor);

	this->ProcessEvent(L"GetDistanceTo", &params);

	return params.ReturnValue;
}

float AActor::GetDistanceTo2D(AActor* OtherActor)
{
	auto Location = GetActorLocation();
	auto OtherLocation = OtherActor->GetActorLocation();
	
	float DeltaX = Location.X - OtherLocation.X;
	float DeltaY = Location.Y - OtherLocation.Y;

	return FMath::Sqrt((DeltaX * DeltaX) + (DeltaY * DeltaY));
}

void AActor::SetActorLocation(FVector NewLocation, bool bSweep, bool bTeleport)
{
	struct
	{
		FVector NewLocation;
		bool bSweep;
		bool bTeleport;
		FHitResult HitResult;
	} params(NewLocation, bSweep, bTeleport);

	this->ProcessEvent(L"K2_SetActorLocation", &params);
}

void AActor::SetActorRotation(FRotator NewRotation, bool bSweep, bool bTeleport)
{
	struct
	{
		FRotator NewRotation;
		bool bSweep;
		bool bTeleport;
		FHitResult HitResult;
	} params(NewRotation, bSweep, bTeleport);

	this->ProcessEvent(L"K2_SetActorRotation", &params);
}

bool AActor::TeleportTo(FVector DestLocation, FRotator DestRotation)
{
	struct
	{
		FVector DestLocation;
		FRotator DestRotation;
		bool ReturnValue;
	} params(DestLocation, DestRotation);

	this->ProcessEvent(L"K2_TeleportTo", &params);

	return params.ReturnValue;
}

void AActor::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation)
{
	struct
	{
		FVector OutLocation;
		FRotator OutRotation;
	}params;

	this->ProcessEvent(L"GetActorEyesViewPoint", &params);

	OutLocation = params.OutLocation;
	OutRotation = params.OutRotation;
}

void AActor::DestroyActor()
{
	this->ProcessEvent(L"K2_DestroyActor");
}

void AActor::SetReplicateMovement(bool bReplicateMovement)
{
	this->ProcessEvent(L"SetReplicateMovement", &bReplicateMovement);
}

void AActor::OnRep_ReplicateMovement()
{
	this->ProcessEvent(L"OnRep_ReplicateMovement");
}