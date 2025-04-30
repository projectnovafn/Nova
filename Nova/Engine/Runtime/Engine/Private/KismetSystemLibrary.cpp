#include "Kismet/KismetSystemLibrary.h"

#include "Kismet/KismetTextLibrary.h"

#include "GameFramework/PlayerController.h"

void UKismetSystemLibrary::PrintString(UObject* WorldObjectContext, FString& InString, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration)
{
	FString SourceObject;
	if (WorldObjectContext != NULL)
	{
		SourceObject = WorldObjectContext->GetName();
	}
	else 
	{
		SourceObject = FString(L"None");
	}

	UE_LOG(LogBlueprintUserMessages, Log, TEXT("[%s] %s"), *SourceObject, *InString);
}

void UKismetSystemLibrary::PrintText(UObject* WorldContextObject, FText InText, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration)
{
	FString Text = UKismetTextLibrary::Conv_TextToString(InText);
	PrintString(WorldContextObject, Text, true, true, FLinearColor(), 0.0f);
}

void UKismetSystemLibrary::PrintWarning(FString& InString)
{
	PrintString(NULL, InString, true, true, FLinearColor(), 0.0f);
}

void UKismetSystemLibrary::ExecuteConsoleCommand(UObject* WorldContextObject, FString Command, APlayerController* SpecificPlayer)
{
	struct
	{
		UObject* WorldContextObject;
		FString Command;
		APlayerController* SpecificPlayer;
	} params(WorldContextObject, Command, SpecificPlayer);

	GetDefaultObject<UKismetSystemLibrary>()->ProcessEvent(L"ExecuteConsoleCommand", &params);
}

bool UKismetSystemLibrary::SphereOverlapActors(UObject* WorldContextObject, FVector SpherePos, float SphereRadius, TArray<EObjectTypeQuery> ObjectTypes, UClass* ActorClassFilter, TArray<AActor*> ActorsToIgnore, TArray<AActor*>* OutActors)
{
	struct
	{
		UObject* WorldContextObject;
		FVector SpherePos;
		float SphereRadius;
		TArray<EObjectTypeQuery> ObjectTypes;
		UClass* ActorClassFilter;
		TArray<AActor*> ActorsToIgnore;
		TArray<AActor*>* OutActors;
	} params(WorldContextObject, SpherePos, SphereRadius, ObjectTypes, ActorClassFilter, ActorsToIgnore, OutActors);

	GetDefaultObject<UKismetSystemLibrary>()->ProcessEvent(L"SphereOverlapActors", &params);
}

FString UKismetSystemLibrary::GetEngineVersion()
{
	static auto GetEngineVersion = UKismetMemoryLibrary::Get<FString* (*)(FString*)>(L"GetEngineVersion");

	FString Temp;
	return *GetEngineVersion(&Temp);
}