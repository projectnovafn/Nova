#pragma once
#include "Algo/Sort.h"
#include "Traits/IsContiguousContainer.h"

/**
 * Helper class for dereferencing pointer types in Sort function
 */
template<typename T, class PREDICATE_CLASS>
struct TDereferenceWrapper
{
    const PREDICATE_CLASS& Predicate;

    TDereferenceWrapper(const PREDICATE_CLASS& InPredicate)
        : Predicate(InPredicate) {}

    /** Pass through for non-pointer types */
    FORCEINLINE bool operator()(T& A, T& B) { return Predicate(A, B); }
    FORCEINLINE bool operator()(const T& A, const T& B) const { return Predicate(A, B); }
};
/** Partially specialized version of the above class */
template<typename T, class PREDICATE_CLASS>
struct TDereferenceWrapper<T*, PREDICATE_CLASS>
{
    const PREDICATE_CLASS& Predicate;

    TDereferenceWrapper(const PREDICATE_CLASS& InPredicate)
        : Predicate(InPredicate) {}

    /** Dereference pointers */
    FORCEINLINE bool operator()(T* A, T* B) const
    {
        return Predicate(*A, *B);
    }
};

/**
 * Wraps a range into a container like interface to satisfy the GetData and GetNum global functions.
 * We're not using TArrayView since it calls ::Sort creating a circular dependency.
 */
template <typename T>
struct TArrayRange
{
    TArrayRange(T* InPtr, int32 InSize)
        : Begin(InPtr)
        , Size(InSize)
    {
    }

    T* GetData() const { return Begin; }
    int32 Num() const { return Size; }

private:
    T* Begin;
    int32 Size;
};

template <typename T>
struct TIsContiguousContainer< TArrayRange<T> >
{
    enum { Value = true };
};

/**
 * Sort elements using user defined predicate class. The sort is unstable, meaning that the ordering of equal items is not necessarily preserved.
 *
 * @param	First	pointer to the first element to sort
 * @param	Num		the number of items to sort
 * @param Predicate predicate class
 */
template<class T, class PREDICATE_CLASS>
void Sort(T* First, const int32 Num, const PREDICATE_CLASS& Predicate)
{
    TArrayRange<T> ArrayRange(First, Num);
    Algo::Sort(ArrayRange, TDereferenceWrapper<T, PREDICATE_CLASS>(Predicate));
}

/**
 * Specialized version of the above Sort function for pointers to elements.
 *
 * @param	First	pointer to the first element to sort
 * @param	Num		the number of items to sort
 * @param Predicate predicate class
 */
template<class T, class PREDICATE_CLASS>
void Sort(T** First, const int32 Num, const PREDICATE_CLASS& Predicate)
{
    TArrayRange<T*> ArrayRange(First, Num);
    Algo::Sort(ArrayRange, TDereferenceWrapper<T*, PREDICATE_CLASS>(Predicate));
}