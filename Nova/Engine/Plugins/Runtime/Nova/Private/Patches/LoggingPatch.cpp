#include "Patches/LoggingPatch.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KismetDetoursLibrary.h"

static void (*_PrintString)(UObject*, FFrame&);
static void (*_PrintWarning)(UObject*, FFrame&);
static void (*_PrintText)(UObject*, FFrame&);

struct PrintStringParams 
{
	UObject* WorldObjectContext;
	FString InString;
	bool bPrintToScreen;
	bool bPrintToLog;
	FLinearColor TextColor;
	float Duration;
};

void LoggingPatch::hkPrintString(UObject* Ctx, FFrame& Stack)
{
	_PrintString(Ctx, Stack);
	auto Params = (PrintStringParams*)Stack.Locals;
	UKismetSystemLibrary::PrintString(Params->WorldObjectContext, Params->InString, Params->bPrintToLog, Params->bPrintToLog, Params->TextColor, Params->Duration);
}

void LoggingPatch::hkPrintWarning(FString& InString)
{
	UKismetSystemLibrary::PrintWarning(InString);
}

void LoggingPatch::hkPrintText(UObject* WorldObjectContext, FText InText, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration)
{
	UKismetSystemLibrary::PrintText(WorldObjectContext, InText, bPrintToLog, bPrintToLog, TextColor, Duration);
}

void LoggingPatch::Init()
{
	/*UKismetDetoursLibrary::AddReflectedDetour<UKismetSystemLibrary>(
		L"PrintString",
		EReflectedDetourType::ExecSwap,
		hkPrintString,
		_PrintString
	);

	UKismetDetoursLibrary::AddReflectedDetour<UKismetSystemLibrary>(
		L"PrintWarning",
		EReflectedDetourType::ImplSwap,
		hkPrintWarning,
		_PrintWarning,
		0,
		{ 0x48, 0x03 }
	);

	UKismetDetoursLibrary::AddReflectedDetour<UKismetSystemLibrary>(
		L"PrintText",
		EReflectedDetourType::ImplSwap,
		hkPrintText,
		_PrintText,
		0,
		{ 0xF3, 0x0F }
	);*/
}