#pragma once
#include "IsPODType.h"
#include "RemoveCV.h"
#include "AreTypesEqual.h"
#include "IsTriviallyCopyConstructible.h"
#include "IsEnum.h"

/** Is type DerivedType inherited from BaseType. */
template<typename DerivedType, typename BaseType>
struct TIsDerivedFrom
{
    // Different size types so we can compare their sizes later.
    typedef char No[1];
    typedef char Yes[2];

    // Overloading Test() s.t. only calling it with something that is
    // a BaseType (or inherited from the BaseType) will return a Yes.
    static Yes& Test(BaseType*);
    static Yes& Test(const BaseType*);
    static No& Test(...);

    // Makes a DerivedType ptr.
    static DerivedType* DerivedTypePtr() { return nullptr; }

public:
    // Test the derived type pointer. If it inherits from BaseType, the Test( BaseType* )
    // will be chosen. If it does not, Test( ... ) will be chosen.
    static const bool IsDerived = sizeof(Test(DerivedTypePtr())) == sizeof(Yes);
};

/**
 * TIsSame
 *
 * Unreal implementation of std::is_same trait.
 */
template<typename A, typename B>	struct TIsSame { enum { Value = false }; };
template<typename T>				struct TIsSame<T, T> { enum { Value = true }; };

/**
 * TIsCharType
 */
template<typename T> struct TIsCharType { enum { Value = false }; };
template<>           struct TIsCharType<ANSICHAR> { enum { Value = true }; };
template<>           struct TIsCharType<UCS2CHAR> { enum { Value = true }; };
template<>           struct TIsCharType<WIDECHAR> { enum { Value = true }; };

/**
 * TIsReferenceType
 */
template<typename T> struct TIsReferenceType { enum { Value = false }; };
template<typename T> struct TIsReferenceType<T&> { enum { Value = true }; };
template<typename T> struct TIsReferenceType<T&&> { enum { Value = true }; };

/**
 * TIsLValueReferenceType
 */
template<typename T> struct TIsLValueReferenceType { enum { Value = false }; };
template<typename T> struct TIsLValueReferenceType<T&> { enum { Value = true }; };

/**
 * TIsRValueReferenceType
 */
template<typename T> struct TIsRValueReferenceType { enum { Value = false }; };
template<typename T> struct TIsRValueReferenceType<T&&> { enum { Value = true }; };

/**
 * TIsVoidType
 */
template<typename T> struct TIsVoidType { enum { Value = false }; };
template<> struct TIsVoidType<void> { enum { Value = true }; };
template<> struct TIsVoidType<void const> { enum { Value = true }; };
template<> struct TIsVoidType<void volatile> { enum { Value = true }; };
template<> struct TIsVoidType<void const volatile> { enum { Value = true }; };

/**
 * Traits for containers.
 */
template<typename T> struct TContainerTraitsBase
{
    // This should be overridden by every container that supports emptying its contents via a move operation.
    enum { MoveWillEmptyContainer = false };
};

template<typename T> struct TContainerTraits : public TContainerTraitsBase<T> {};

/**
 * Call traits helpers
 */
template <typename T, bool TypeIsSmall>
struct TCallTraitsParamTypeHelper
{
    typedef const T& ParamType;
    typedef const T& ConstParamType;
};
template <typename T>
struct TCallTraitsParamTypeHelper<T, true>
{
    typedef const T ParamType;
    typedef const T ConstParamType;
};
template <typename T>
struct TCallTraitsParamTypeHelper<T*, true>
{
    typedef T* ParamType;
    typedef const T* ConstParamType;
};

template <typename T>
struct TCallTraitsBase
{
private:
    enum { PassByValue = TOr<TAndValue<(sizeof(T) <= sizeof(void*)), TIsPODType<T>>, TIsArithmetic<T>, TIsPointer<T>>::Value };
public:
    typedef T ValueType;
    typedef T& Reference;
    typedef const T& ConstReference;
    typedef typename TCallTraitsParamTypeHelper<T, PassByValue>::ParamType ParamType;
    typedef typename TCallTraitsParamTypeHelper<T, PassByValue>::ConstParamType ConstPointerType;
};

/**
 * TCallTraits
 */
template <typename T>
struct TCallTraits : public TCallTraitsBase<T> {};

template<typename T>
struct TTypeTraitsBase
{
    typedef typename TCallTraits<T>::ParamType ConstInitType;
    typedef typename TCallTraits<T>::ConstPointerType ConstPointerType;

    // There's no good way of detecting this so we'll just assume it to be true for certain known types and expect
    // users to customize it for their custom types.
    enum { IsBytewiseComparable = TOr<TIsEnum<T>, TIsArithmetic<T>, TIsPointer<T>>::Value };
};

/**
 * Traits for types.
 */
template<typename T> struct TTypeTraits : public TTypeTraitsBase<T> {};

template <typename T, typename Arg>
struct TIsBitwiseConstructible
{
    static_assert(
        !TIsReferenceType<T  >::Value &&
        !TIsReferenceType<Arg>::Value,
        "TIsBitwiseConstructible is not designed to accept reference types");

    static_assert(
        TAreTypesEqual<T, typename TRemoveCV<T  >::Type>::Value&&
        TAreTypesEqual<Arg, typename TRemoveCV<Arg>::Type>::Value,
        "TIsBitwiseConstructible is not designed to accept qualified types");

    // Assume no bitwise construction in general
    enum { Value = false };
};

template <typename T>
struct TIsBitwiseConstructible<T, T>
{
    // Ts can always be bitwise constructed from itself if it is trivially copyable.
    enum { Value = TIsTriviallyCopyConstructible<T>::Value };
};

template <typename T, typename U>
struct TIsBitwiseConstructible<const T, U> : TIsBitwiseConstructible<T, U>
{
    // Constructing a const T is the same as constructing a T
};

// Const pointers can be bitwise constructed from non-const pointers.
// This is not true for pointer conversions in general, e.g. where an offset may need to be applied in the case
// of multiple inheritance, but there is no way of detecting that at compile-time.
template <typename T>
struct TIsBitwiseConstructible<const T*, T*>
{
    // Constructing a const T is the same as constructing a T
    enum { Value = true };
};

// Unsigned types can be bitwise converted to their signed equivalents, and vice versa.
// (assuming two's-complement, which we are)
template <> struct TIsBitwiseConstructible<uint8, int8> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible< int8, uint8> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible<uint16, int16> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible< int16, uint16> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible<uint32, int32> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible< int32, uint32> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible<uint64, int64> { enum { Value = true }; };
template <> struct TIsBitwiseConstructible< int64, uint64> { enum { Value = true }; };

/**
 * TIsFunction
 *
 * Tests is a type is a function.
 */
template <typename T>
struct TIsFunction
{
    enum { Value = false };
};

template <typename RetType, typename... Params>
struct TIsFunction<RetType(Params...)>
{
    enum { Value = true };
};

template<typename T>
struct TIsZeroConstructType
{
    enum { Value = TOr<TIsEnum<T>, TIsArithmetic<T>, TIsPointer<T>>::Value };
};

template<typename T>
struct TIsWeakPointerType
{
    enum { Value = false };
};