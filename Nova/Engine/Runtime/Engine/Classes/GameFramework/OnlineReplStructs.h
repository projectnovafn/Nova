#pragma once
#include "CoreUObject.h"

class FAccountId
{
private:
    void** VTable;

public:

    bool IsValid()
    {
        return ((bool (*)(void*))(VTable[4]))(this);
    }

    FString ToString()
    {
        FString Out;
        ((void (*)(void*, FString&))(VTable[5]))(this, Out);

        return Out;
    }
};

class FUniqueNetIdRepl
{
	GENERATED_USTRUCT_BODY(FUniqueNetIdRepl, Engine)
private:

    void** VTable;
    FAccountId* Variant;

    unsigned char Pad[0x17];

public:

    FString ToString()
    {
        if (Variant && Variant->IsValid())
        {
            return Variant->ToString();
        }

        return {};
    }
};