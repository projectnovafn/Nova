#include "Engine/GameplayStatics.h"

AActor* UGameplayStatics::BeginSpawningActorFromClass(UObject* WorldContextObject, UClass* ActorClass, FTransform SpawnTransform, bool bNoCollisionFail, AActor* Owner)
{
	struct
	{
		UObject* WorldContextObject;
		UClass* ActorClass;
		FTransform SpawnTransform;
		bool bNoCollisionFail;
		AActor* Owner;
		AActor* ReturnValue;
	} params(WorldContextObject, ActorClass, SpawnTransform, bNoCollisionFail, Owner);

	GetDefaultObject<UGameplayStatics>()->ProcessEvent(L"BeginSpawningActorFromClass", &params);

	return params.ReturnValue;
}

AActor* UGameplayStatics::FinishSpawningActor(AActor* Actor, FTransform SpawnTransform)
{
	struct
	{
		AActor* Actor;
		FTransform SpawnTransform;
		AActor* ReturnValue;
	} params(Actor, SpawnTransform);

	GetDefaultObject<UGameplayStatics>()->ProcessEvent(L"FinishSpawningActor", &params);

	return params.ReturnValue;
}

UObject* UGameplayStatics::SpawnObject(UClass* ObjectClass, UObject* Outer)
{
	struct
	{
		UClass* ObjectClass;
		UObject* Outer;
		UObject* ReturnValue;
	} params(ObjectClass, Outer);

	GetDefaultObject<UGameplayStatics>()->ProcessEvent(L"SpawnObject", &params);

	return params.ReturnValue;
}

TArray<AActor*> UGameplayStatics::GetAllActorsOfClass(UObject* WorldContextObject, UClass* ActorClass)
{
	struct
	{
		UObject* WorldContextObject;
		UClass* ActorClass;
		TArray<AActor*> OutActors;
	} params(WorldContextObject, ActorClass);

	GetDefaultObject<UGameplayStatics>()->ProcessEvent(L"GetAllActorsOfClass", &params);

	return params.OutActors;
}

void UGameplayStatics::OpenLevel(UObject* WorldContextObject, FName LevelName, bool bAbsolute, FString Options)
{
	struct
	{
		UObject* WorldContextObject;
		FName LevelName;
		bool bAbsolute;
		FString Options;
	} params(WorldContextObject, LevelName, bAbsolute, Options);

	GetDefaultObject<UGameplayStatics>()->ProcessEvent(L"OpenLevel", &params);
}

float UGameplayStatics::GetTimeSeconds(UObject* WorldContextObject)
{
	struct
	{
		UObject* WorldContextObject;
		float ReturnValue;
	} params(WorldContextObject);

	GetDefaultObject<UGameplayStatics>()->ProcessEvent(L"GetTimeSeconds", &params);

	return params.ReturnValue;
}