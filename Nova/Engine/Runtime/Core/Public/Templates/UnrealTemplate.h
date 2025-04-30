#pragma once
#include "AreTypesEqual.h"
#include "Traits/IsContiguousContainer.h"
#include "RemoveReference.h"

template <typename From, typename To> struct TCopyQualifiersFromTo { typedef                To Type; };
template <typename From, typename To> struct TCopyQualifiersFromTo<const          From, To> { typedef const          To Type; };
template <typename From, typename To> struct TCopyQualifiersFromTo<      volatile From, To> { typedef       volatile To Type; };
template <typename From, typename To> struct TCopyQualifiersFromTo<const volatile From, To> { typedef const volatile To Type; };

template <typename From, typename To>
struct TLosesQualifiersFromTo
{
    enum { Value = !TAreTypesEqual<typename TCopyQualifiersFromTo<From, To>::Type, To>::Value };
};

template <typename T> struct TRValueToLValueReference { typedef T  Type; };
template <typename T> struct TRValueToLValueReference<T&&> { typedef T& Type; };

class FNoncopyable
{
protected:
    // ensure the class cannot be constructed directly
    FNoncopyable() {}
    // the class should not be used polymorphically
    ~FNoncopyable() {}
private:
    FNoncopyable(const FNoncopyable&);
    FNoncopyable& operator=(const FNoncopyable&);
};

/**
 * Generically gets the data pointer of a contiguous container
 */
template<typename T, typename = typename TEnableIf<TIsContiguousContainer<T>::Value>::Type>
auto GetData(T&& Container) -> decltype(Container.GetData())
{
    return Container.GetData();
}

template <typename T, SIZE_T N>
CONSTEXPR T* GetData(T(&Container)[N])
{
    return Container;
}

template <typename T>
CONSTEXPR T* GetData(std::initializer_list<T> List)
{
    return List.begin();
}

/**
* Generically gets the number of items in a contiguous container
*/
template<typename T, typename = typename TEnableIf<TIsContiguousContainer<T>::Value>::Type>
SIZE_T GetNum(T&& Container)
{
    return (SIZE_T)Container.Num();
}

template <typename T, SIZE_T N>
CONSTEXPR SIZE_T GetNum(T(&Container)[N])
{
    return N;
}

template <typename T>
CONSTEXPR SIZE_T GetNum(std::initializer_list<T> List)
{
    return List.size();
}

template <typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& MoveTempIfPossible(T&& Obj)
{
    typedef typename TRemoveReference<T>::Type CastType;
    return (CastType&&)Obj;
}

/**
 * This exists to avoid a Visual Studio bug where using a cast to forward an rvalue reference array argument
 * to a pointer parameter will cause bad code generation.  Wrapping the cast in a function causes the correct
 * code to be generated.
 */
template <typename T, typename ArgType>
FORCEINLINE T StaticCast(ArgType&& Arg)
{
    return static_cast<T>(Arg);
}

/**
 * Reverses the order of the bits of a value.
 * This is an TEnableIf'd template to ensure that no undesirable conversions occur.  Overloads for other types can be added in the same way.
 *
 * @param Bits - The value to bit-swap.
 * @return The bit-swapped value.
 */
template <typename T>
FORCEINLINE typename TEnableIf<TAreTypesEqual<T, uint32>::Value, T>::Type ReverseBits(T Bits)
{
    Bits = (Bits << 16) | (Bits >> 16);
    Bits = ((Bits & 0x00ff00ff) << 8) | ((Bits & 0xff00ff00) >> 8);
    Bits = ((Bits & 0x0f0f0f0f) << 4) | ((Bits & 0xf0f0f0f0) >> 4);
    Bits = ((Bits & 0x33333333) << 2) | ((Bits & 0xcccccccc) >> 2);
    Bits = ((Bits & 0x55555555) << 1) | ((Bits & 0xaaaaaaaa) >> 1);
    return Bits;
}

/**
 * Forward will cast a reference to an rvalue reference.
 * This is UE's equivalent of std::forward.
 */
template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type& Obj)
{
    return (T&&)Obj;
}

template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type&& Obj)
{
    return (T&&)Obj;
}

/**
 * A traits class which specifies whether a Swap of a given type should swap the bits or use a traditional value-based swap.
 */
template <typename T>
struct TUseBitwiseSwap
{
    // We don't use bitwise swapping for 'register' types because this will force them into memory and be slower.
    enum { Value = !TOrValue<__is_enum(T), TIsPointer<T>, TIsArithmetic<T>>::Value };
};

template <typename T>
inline typename TEnableIf<!TUseBitwiseSwap<T>::Value>::Type Swap(T& A, T& B)
{
    T Temp = MoveTemp(A);
    A = MoveTemp(B);
    B = MoveTemp(Temp);
}

template <typename T>
inline void Exchange(T& A, T& B)
{
    Swap(A, B);
}

/**
 * MoveTemp will cast a reference to an rvalue reference.
 * This is UE's equivalent of std::move except that it will not compile when passed an rvalue or
 * const object, because we would prefer to be informed when MoveTemp will have no effect.
 */
template <typename T>
FORCEINLINE typename TRemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
    typedef typename TRemoveReference<T>::Type CastType;

    // Validate that we're not being passed an rvalue or a const object - the former is redundant, the latter is almost certainly a mistake
    static_assert(TIsLValueReferenceType<T>::Value, "MoveTemp called on an rvalue");
    static_assert(!TAreTypesEqual<CastType&, const CastType&>::Value, "MoveTemp called on a const object");

    return (CastType&&)Obj;
}