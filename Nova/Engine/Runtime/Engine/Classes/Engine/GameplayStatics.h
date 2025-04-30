#pragma once
#include "CoreUObject.h"

#include "GameFramework/Actor.h"

class UGameplayStatics : public UObject
{
	GENERATED_UCLASS_BODY(UGameplayStatics, Engine)

public:

	static AActor* BeginSpawningActorFromClass(UObject* WorldContextObject, UClass* ActorClass, FTransform SpawnTransform, bool bNoCollisionFail = false, AActor* Owner = NULL);
	static AActor* FinishSpawningActor(AActor* Actor, FTransform SpawnTransform);
	static TArray<AActor*> GetAllActorsOfClass(UObject* WorldContextObject, UClass* ActorClass);
	static float GetTimeSeconds(UObject* WorldContextObject);

	static void OpenLevel(UObject* WorldContextObject, FName LevelName, bool bAbsolute, FString Options);

	template <typename T>
	static TArray<T*> GetAllActorsOfClass(UObject* WorldContextObject, UClass* ActorClass = T::StaticClass())
	{
		auto Actors = UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ActorClass);
		return *(TArray<T*>*)(&Actors);
	}

	static UObject* SpawnObject(UClass* ObjectClass, UObject* Outer);

	template <typename T>
	static T* SpawnObject(UObject* Outer = GetTransientPackage())
	{
		return (T*)UGameplayStatics::SpawnObject(T::StaticClass(), Outer);
	}
};