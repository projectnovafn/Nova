#pragma once
#include "CoreTypes.h"
#include <string>

/** this is the character used to separate a subobject root from its subobjects in a path name. */
#define SUBOBJECT_DELIMITER				TEXT(":")

/** this is the character used to separate a subobject root from its subobjects in a path name, as a char */
#define SUBOBJECT_DELIMITER_CHAR		':'

enum EFindName
{
    FNAME_Find,
    FNAME_Add,
    FNAME_Replace_Not_Safe_For_Threading,
};

class FName
{
public:
    union
    {
        struct
        {
            int32 ComparisonIndex;
            int32 Number;
        };

        uint64 CompositeComparisonValue;
    };
public:

    FORCEINLINE FName(int32 Index = 0, int32 Number = 0) : ComparisonIndex(Index), Number(Number) {}

    FName(const TCHAR* Value, EFindName FindType = EFindName::FNAME_Find);
    class FString ToString() const;


    FORCEINLINE bool operator==(const FName& Other) const { return CompositeComparisonValue == Other.CompositeComparisonValue; }
    FORCEINLINE bool operator!=(const FName& Other) const
    {
        return !(*this == Other);
    }
};

inline uint32 GetTypeHash(const FName N)
{
    return N.ComparisonIndex + N.Number;
}

inline bool operator<(const FName& l, const FName& r) {
    return (l.ComparisonIndex < r.ComparisonIndex || (l.ComparisonIndex == r.ComparisonIndex && l.Number < r.Number));
}