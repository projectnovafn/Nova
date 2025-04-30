#include "Kismet/KismetTextLibrary.h"

FText UKismetTextLibrary::Conv_StringToText(const FString& InString)
{
	struct
	{
		FString InString;
		FText ReturnValue;
	}params(InString);

	GetDefaultObject<UKismetTextLibrary>()->ProcessEvent(L"Conv_StringToText", &params);

	return params.ReturnValue;
}

FString UKismetTextLibrary::Conv_TextToString(const FText& InText)
{
	struct
	{
		FText InText;
		FString ReturnValue;
	}params(InText);

	GetDefaultObject<UKismetTextLibrary>()->ProcessEvent(L"Conv_TextToString", &params);

	return params.ReturnValue;
}
