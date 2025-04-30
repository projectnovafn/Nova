#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

/**
 * Metafunction which detects whether or not a class is an IInterface.  Rules:
 *
 * 1. A UObject is not an IInterface.
 * 2. A type without a UClassType typedef member is not an IInterface.
 * 3. A type whose UClassType::StaticClassFlags does not have CLASS_Interface set is not an IInterface.
 *
 * Otherwise, assume it's an IInterface.
 */
template <typename T, bool bIsAUObject_IMPL = TPointerIsConvertibleFromTo<T, const volatile UObject>::Value>
struct TIsIInterface
{
    enum { Value = false };
};

template <typename T>
struct TIsIInterface<T, false>
{
    enum { Value = sizeof(Resolve<T>(0)) - 1 };
};

template <typename T>
struct TIsCastable
{
    // It's from-castable if it's an interface or a UObject-derived type
    enum { Value = TIsIInterface<T>::Value || TPointerIsConvertibleFromTo<T, const volatile UObject>::Value };
};

template <typename T>
struct TIsCastableToPointer : TOr<TIsVoidType<T>, TIsCastable<T>>
{
    // It's to-pointer-castable if it's from-castable or void
};

template <typename T>
class THasIsA
{
    using YesType = char[1];
    using NoType = char[2];

    template <typename C> static YesType& Test(decltype(&C::template IsA<>)*);
    template <typename C> static NoType& Test(...);

public:
    enum { Value = sizeof(Test<T>(0)) == sizeof(YesType) };
};

template <typename To, typename From>
FORCEINLINE To* Cast(From* Src)
{
    return Src->IsValid() && Src->template IsA<To>() ? static_cast<To*>(Src) : nullptr;
}

template <typename To, typename From>
To* CastChecked(From* Src, ECastCheckedType::Type CheckType = ECastCheckedType::NullChecked)
{
    if (Src)
    {
        return Cast<To>(Src);
    }

    return nullptr;
}
