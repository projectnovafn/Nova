#pragma once
#include "UObject/Stack.h"

class LoggingPatch
{
private:
	static void hkPrintString(UObject*, FFrame&);
	static void hkPrintWarning(FString&);
	static void hkPrintText(UObject* WorldContextObject, FText InText, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration);

public:
	static void Init();
};