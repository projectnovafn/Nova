#pragma once

class FString;

class FText
{
private:
    char Pad[0x18];
public:

    FText();
    FText(const FString& InString);

    FString ToString();
};