#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include <Misc/Crc.h>
#include <Templates/UnrealTemplate.h>
#include "HAL/PlatformString.h"
#include "Misc/CString.h"
#include "UObject/NameTypes.h"

/** Determines case sensitivity options for string comparisons. */
namespace ESearchCase
{
    enum Type
    {
        /** Case sensitive. Upper/lower casing must match for strings to be considered equal. */
        CaseSensitive,

        /** Ignore case. Upper/lower casing does not matter when making a comparison. */
        IgnoreCase,
    };
};

/** Determines search direction for string operations. */
namespace ESearchDir
{
    enum Type
    {
        /** Search from the start, moving forward through the string. */
        FromStart,

        /** Search from the end, moving backward through the string. */
        FromEnd,
    };
}

class FString
{
private:
    typedef TArray<WIDECHAR> DataType;
    DataType Data;
public:
    __forceinline wchar_t* c_str() { return Data.GetData(); }

public:
    FString() = default;
    FString(FString&&) = default;
    FString(const FString&) = default;
    FString& operator=(FString&&) = default;
    FString& operator=(const FString&) = default;

    FORCEINLINE explicit FString(int32 InCount, const wchar_t* InSrc)
    {
        Data.AddUninitialized(InCount ? InCount + 1 : 0);

        if (Data.Num() > 0)
        {
            FCString::Strncpy(Data.GetData(), InSrc, InCount + 1);
        }
    }

    FORCEINLINE FString(const WIDECHAR* Src)
    {
        if (Src && *Src)
        {
            int32 SrcLen = TCString<WIDECHAR>::Strlen(Src) + 1;
            int32 DestLen = FPlatformString::ConvertedLength<TCHAR>(Src, SrcLen);
            Data.AddUninitialized(DestLen);

            FPlatformString::Convert(Data.GetData(), DestLen, Src, SrcLen);
        }
    }

    FORCEINLINE FString(const std::string& Src) : FString(std::wstring(Src.begin(), Src.end()).c_str())
    {
    }

    FORCEINLINE const TCHAR* operator*() const
    {
        return Data.Num() ? Data.GetData() : L"";
    }

    const TCHAR* CStr() const;

    FORCEINLINE std::string ToString() const
    {
        auto wstring = std::wstring(Data.GetData());
        return std::string(wstring.begin(), wstring.end());
    }

    FORCEINLINE friend bool operator==(const FString& Lhs, const FString& Rhs)
    {
        return FPlatformString::Stricmp(*Lhs, *Rhs) == 0;
    }

    template <typename CharType>
    FORCEINLINE friend bool operator==(const FString& Lhs, const CharType* Rhs)
    {
        return FPlatformString::Stricmp(*Lhs, Rhs) == 0;
    }

    template <typename CharType>
    FORCEINLINE friend bool operator==(const CharType* Lhs, const FString& Rhs)
    {
        return FPlatformString::Stricmp(Lhs, *Rhs) == 0;
    }

    FORCEINLINE friend bool operator!=(const FString& Lhs, const FString& Rhs)
    {
        return FPlatformString::Stricmp(*Lhs, *Rhs) != 0;
    }

    template <typename CharType>
    FORCEINLINE friend bool operator!=(const FString& Lhs, const CharType* Rhs)
    {
        return FPlatformString::Stricmp(*Lhs, Rhs) != 0;
    }

    template <typename CharType>
    FORCEINLINE friend bool operator!=(const CharType* Lhs, const FString& Rhs)
    {
        return FPlatformString::Stricmp(Lhs, *Rhs) != 0;
    }

    /**
 * Return specific character from this string
 *
 * @param Index into string
 * @return Character at Index
 */
    FORCEINLINE WIDECHAR& operator[](int32 Index)
    {
        //checkf(IsValidIndex(Index), TEXT("String index out of bounds: Index %i from a string with a length of %i"), Index, Len());
        return Data.GetData()[Index];
    }

    /**
     * Return specific const character from this string
     *
     * @param Index into string
     * @return const Character at Index
     */
    FORCEINLINE const WIDECHAR& operator[](int32 Index) const
    {
        //checkf(IsValidIndex(Index), TEXT("String index out of bounds: Index %i from a string with a length of %i"), Index, Len());
        return Data.GetData()[Index];
    }

    /**
     * Empties the string, but doesn't change memory allocation, unless the new size is larger than the current string.
     *
     * @param NewReservedSize The expected usage size (in characters, not including the terminator) after calling this function.
     */
    FORCEINLINE void Reset(int32 NewReservedSize = 0)
    {
        const int32 NewSizeIncludingTerminator = (NewReservedSize > 0) ? (NewReservedSize + 1) : 0;
        Data.Reset(NewSizeIncludingTerminator);
        if (WIDECHAR* DataPtr = Data.GetData())
        {
            *DataPtr = TEXT('\0');
        }
    }

    FORCEINLINE bool IsEmpty() const
    {
        return Data.Num() <= 1;
    }

    FORCEINLINE void Empty(int32 Slack = 0)
    {
        Data.Empty(Slack);
    }

    /**
     * Concatenate this with given char
     *
     * @param InChar other Char to be concatenated onto the end of this string
     * @return reference to this
     */
    FORCEINLINE FString& AppendChar(const TCHAR InChar)
    {
        *this += InChar;
        return *this;
    }

    FORCEINLINE FString& Append(const FString& Text)
    {
        *this += Text;
        return *this;
    }

    FORCEINLINE FString& operator+=(const FString& Str)
    {
        //CheckInvariants();
        //Str.CheckInvariants();

        AppendChars(Str.Data.GetData(), Str.Len());

        return *this;
    }

    template <typename CharType>
    FORCEINLINE typename TEnableIf<TIsCharType<CharType>::Value, FString&>::Type operator+=(CharType InChar)
    {
        //CheckInvariants();

        if (InChar != 0)
        {
            // position to insert the character.
            // At the end of the string if we have existing characters, otherwise at the 0 position
            int32 InsertIndex = (Data.Num() > 0) ? Data.Num() - 1 : 0;

            // number of characters to add.  If we don't have any existing characters,
            // we'll need to append the terminating zero as well.
            int32 InsertCount = (Data.Num() > 0) ? 1 : 2;

            Data.AddUninitialized(InsertCount);
            Data[InsertIndex] = InChar;
            Data[InsertIndex + 1] = 0;
        }
        return *this;
    }

    FORCEINLINE void AppendChars(const WIDECHAR* Array, int32 Count)
    {
        check(Count >= 0);

        if (!Count)
            return;

        checkSlow(Array);

        int32 Index = Data.Num();

        // Reserve enough space - including an extra gap for a null terminator if we don't already have a string allocated
        Data.AddUninitialized(Count + (Index ? 0 : 1));

        WIDECHAR* EndPtr = Data.GetData() + Index - (Index ? 1 : 0);

        // Copy characters to end of string, overwriting null terminator if we already have one
        CopyAssignItems(EndPtr, Array, Count);

        // (Re-)establish the null terminator
        *(EndPtr + Count) = 0;
    }

    FString& Append(const TCHAR* Text, int32 Count)
    {
        //CheckInvariants();

        if (Count != 0)
        {
            // position to insert the character.
            // At the end of the string if we have existing characters, otherwise at the 0 position
            int32 InsertIndex = (Data.Num() > 0) ? Data.Num() - 1 : 0;

            // number of characters to add.  If we don't have any existing characters,
            // we'll need to append the terminating zero as well.
            int32 FinalCount = (Data.Num() > 0) ? Count : Count + 1;

            Data.AddUninitialized(FinalCount);

            for (int32 Index = 0; Index < Count; Index++)
            {
                Data[InsertIndex + Index] = Text[Index];
            }

            Data[Data.Num() - 1] = 0;
        }
        return *this;
    }

    FORCEINLINE int32 Len() const
    {
        return Data.Num() ? Data.Num() - 1 : 0;
    }

    /** @return a new string with the characters of this converted to uppercase */
    FString ToUpper() const&;

    /**
     * Converts all characters in this rvalue string to uppercase and moves it into the returned string.
     * @return a new string with the characters of this converted to uppercase
     */
    FString ToUpper()&&;

    /** Converts all characters in this string to uppercase */
    void ToUpperInline();

    /** @return a new string with the characters of this converted to lowercase */
    FString ToLower() const&;

    /**
     * Converts all characters in this rvalue string to lowercase and moves it into the returned string.
     * @return a new string with the characters of this converted to lowercase
     */
    FString ToLower()&&;

    /** Converts all characters in this string to lowercase */
    void ToLowerInline();

    /** @return the left most given number of characters */
    FORCEINLINE FString Left(int32 Count) const
    {
        return FString(FMath::Clamp(Count, 0, Len()), **this);
    }

    /** @return the left most characters from the string chopping the given number of characters from the end */
    FORCEINLINE FString LeftChop(int32 Count) const
    {
        return FString(FMath::Clamp(Len() - Count, 0, Len()), **this);
    }

    /** @return the string to the right of the specified location, counting back from the right (end of the word). */
    FORCEINLINE FString Right(int32 Count) const
    {
        return FString(**this + Len() - FMath::Clamp(Count, 0, Len()));
    }

    /** @return the string to the right of the specified location, counting forward from the left (from the beginning of the word). */
    FORCEINLINE FString RightChop(int32 Count) const
    {
        return FString(**this + Len() - FMath::Clamp(Len() - Count, 0, Len()));
    }

    /** @return the substring from Start position for Count characters. */
    FORCEINLINE FString Mid(int32 Start, int32 Count = MAX_int32) const
    {
        check(Count >= 0);
        uint32 End = Start + Count;
        Start = FMath::Clamp((uint32)Start, (uint32)0, (uint32)Len());
        End = FMath::Clamp((uint32)End, (uint32)Start, (uint32)Len());
        return FString(End - Start, **this + Start);
    }

    /**
     * Searches the string for a substring, and returns index into this string
     * of the first found instance. Can search from beginning or end, and ignore case or not.
     *
     * @param SubStr			The string array of TCHAR to search for
     * @param StartPosition		The start character position to search from
     * @param SearchCase		Indicates whether the search is case sensitive or not
     * @param SearchDir			Indicates whether the search starts at the begining or at the end.
     */
    int32 Find(const WIDECHAR* SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
        ESearchDir::Type SearchDir = ESearchDir::FromStart, int32 StartPosition = INDEX_NONE) const;

    /**
     * Searches the string for a substring, and returns index into this string
     * of the first found instance. Can search from beginning or end, and ignore case or not.
     *
     * @param SubStr			The string to search for
     * @param StartPosition		The start character position to search from
     * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
     * @param SearchDir			Indicates whether the search starts at the begining or at the end ( defaults to ESearchDir::FromStart )
     */
    FORCEINLINE int32 Find(const FString& SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
        ESearchDir::Type SearchDir = ESearchDir::FromStart, int32 StartPosition = INDEX_NONE) const
    {
        return Find(*SubStr, SearchCase, SearchDir, StartPosition);
    }

    bool Split(const FString& InS, FString* LeftS, FString* RightS, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
        ESearchDir::Type SearchDir = ESearchDir::FromStart) const
    {
        int32 InPos = Find(InS, SearchCase, SearchDir);

        if (InPos < 0) { return false; }

        if (LeftS) { *LeftS = Left(InPos); }
        if (RightS) { *RightS = Mid(InPos + InS.Len()); }

        return true;
    }
};

FORCEINLINE uint32 GetTypeHash(const FString& S)
{
    return FCrc::Strihash_DEPRECATED(*S);
}