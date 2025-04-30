#include "Internationalization/Text.h"
#include "Kismet/KismetTextLibrary.h"

FText::FText()
{
    FMemory::Memzero(Pad);
}

FText::FText(const FString& InString)
{
    *this = UKismetTextLibrary::Conv_StringToText(InString);
}

FString FText::ToString()
{
    return UKismetTextLibrary::Conv_TextToString(*this);
}