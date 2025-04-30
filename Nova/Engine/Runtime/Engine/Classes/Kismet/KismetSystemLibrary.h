#pragma once

#include "CoreUObject.h"
#include "Internationalization/Text.h"

class AActor;
class APlayerController;

enum class EObjectTypeQuery : uint8
{
	ObjectTypeQuery1 = 0,
	ObjectTypeQuery2 = 1,
	ObjectTypeQuery3 = 2,
	ObjectTypeQuery4 = 3,
	ObjectTypeQuery5 = 4,
	ObjectTypeQuery6 = 5,
	ObjectTypeQuery7 = 6,
	ObjectTypeQuery8 = 7,
	ObjectTypeQuery9 = 8,
	ObjectTypeQuery10 = 9,
	ObjectTypeQuery11 = 10,
	ObjectTypeQuery12 = 11,
	ObjectTypeQuery13 = 12,
	ObjectTypeQuery14 = 13,
	ObjectTypeQuery15 = 14,
	ObjectTypeQuery16 = 15,
	ObjectTypeQuery17 = 16,
	ObjectTypeQuery18 = 17,
	ObjectTypeQuery19 = 18,
	ObjectTypeQuery20 = 19,
	ObjectTypeQuery21 = 20,
	ObjectTypeQuery22 = 21,
	ObjectTypeQuery23 = 22,
	ObjectTypeQuery24 = 23,
	ObjectTypeQuery25 = 24,
	ObjectTypeQuery26 = 25,
	ObjectTypeQuery27 = 26,
	ObjectTypeQuery28 = 27,
	ObjectTypeQuery29 = 28,
	ObjectTypeQuery30 = 29,
	ObjectTypeQuery31 = 30,
	ObjectTypeQuery32 = 31,
	ObjectTypeQuery_MAX = 32
};

class UKismetSystemLibrary : public UObject
{
    GENERATED_UCLASS_BODY(UKismetSystemLibrary, Engine)

public:

    static void PrintString(UObject* WorldObjectContext, FString& InString, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration);
    static void PrintText(UObject* WorldContextObject, FText InText, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration);
    static void PrintWarning(FString& InString);
    static void ExecuteConsoleCommand(UObject* WorldContextObject, FString Command, APlayerController* SpecificPlayer);
    static bool SphereOverlapActors(UObject* WorldContextObject, FVector SpherePos, float SphereRadius, TArray<EObjectTypeQuery> ObjectTypes, UClass* ActorClassFilter, TArray<AActor*> ActorsToIgnore, TArray<AActor*>* OutActors);

    static FString GetEngineVersion();

    static double GetEngineVersionNum()
    {
        auto EngineVersion = GetEngineVersion().ToString();
        return std::stod(EngineVersion.substr(0, 4));
    }

    static double GetFortniteVersion()
    {
        auto EngineVersion = GetEngineVersion().ToString();
        return std::stod(EngineVersion.substr(EngineVersion.find_last_of("-") + 1, EngineVersion.length() - EngineVersion.find_last_of("-") + 1));
    }
};