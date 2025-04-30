#pragma once

#include "CoreUObject.h"

#include "Internationalization/Text.h"

class UKismetTextLibrary : public UObject
{
	GENERATED_UCLASS_BODY(UKismetTextLibrary, Engine)

public:

	static FText Conv_StringToText(const FString& InString);
	static FString Conv_TextToString(const FText& InText);
};