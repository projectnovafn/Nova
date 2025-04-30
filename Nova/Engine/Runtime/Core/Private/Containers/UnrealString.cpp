#include "Containers/UnrealString.h"

int32 FString::Find(const WIDECHAR* SubStr, ESearchCase::Type SearchCase, ESearchDir::Type SearchDir, int32 StartPosition) const
{
    if (SubStr == nullptr)
    {
        return INDEX_NONE;
    }
    if (SearchDir == ESearchDir::FromStart)
    {
        const WIDECHAR* Start = **this;
        if (StartPosition != INDEX_NONE)
        {
            Start += FMath::Clamp(StartPosition, 0, Len() - 1);
        }
        const WIDECHAR* Tmp = SearchCase == ESearchCase::IgnoreCase
            ? FCString::Stristr(Start, SubStr)
            : FCString::Strstr(Start, SubStr);

        return Tmp ? (Tmp - **this) : INDEX_NONE;
    }
    else
    {
        // if ignoring, do a onetime ToUpper on both strings, to avoid ToUppering multiple
        // times in the loop below
        if (SearchCase == ESearchCase::IgnoreCase)
        {
            return ToUpper().Find(FString(SubStr).ToUpper(), ESearchCase::CaseSensitive, SearchDir, StartPosition);
        }
        else
        {
            const int32 SearchStringLength = FMath::Max(1, FCString::Strlen(SubStr));

            if (StartPosition == INDEX_NONE || StartPosition >= Len())
            {
                StartPosition = Len();
            }

            for (int32 i = StartPosition - SearchStringLength; i >= 0; i--)
            {
                int32 j;
                for (j = 0; SubStr[j]; j++)
                {
                    if ((*this)[i + j] != SubStr[j])
                    {
                        break;
                    }
                }

                if (!SubStr[j])
                {
                    return i;
                }
            }
            return INDEX_NONE;
        }
    }
}

FORCENOINLINE const TCHAR* FString::CStr() const
{
    return Data.Num() ? Data.GetData() : L"";
}

FString FString::ToUpper() const&
{
    FString New = *this;
    New.ToUpperInline();
    return New;
}

FString FString::ToUpper()&&
{
    this->ToUpperInline();
    return MoveTemp(*this);
}

void FString::ToUpperInline()
{
    const int32 StringLength = Len();
    WIDECHAR* RawData = Data.GetData();
    for (int32 i = 0; i < StringLength; ++i)
    {
        RawData[i] = FChar::ToUpper(RawData[i]);
    }
}


FString FString::ToLower() const&
{
    FString New = *this;
    New.ToLowerInline();
    return New;
}

FString FString::ToLower()&&
{
    this->ToLowerInline();
    return MoveTemp(*this);
}

void FString::ToLowerInline()
{
    const int32 StringLength = Len();
    WIDECHAR* RawData = Data.GetData();
    for (int32 i = 0; i < StringLength; ++i)
    {
        RawData[i] = FChar::ToLower(RawData[i]);
    }
}