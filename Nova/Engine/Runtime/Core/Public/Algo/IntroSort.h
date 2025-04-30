#pragma once

#include "CoreTypes.h"
#include "HAL/PlatformMath.h"
#include "Templates/IdentityFunctor.h"
#include "Templates/Less.h"
#include "Impl/BinaryHeap.h"

namespace AlgoImpl
{
    /**
     * Implementation of an introspective sort. Starts with quick sort and switches to heap sort when the iteration depth is too big.
     * The sort is unstable, meaning that the ordering of equal items is not necessarily preserved.
     * This is the internal sorting function used by IntroSort overrides.
     *
     * @param First			pointer to the first element to sort
     * @param Num			the number of items to sort
     * @param Projection	The projection to sort by when applied to the element.
     * @param Predicate		predicate class
     */
    template <typename T, typename ProjectionType, typename PredicateType>
    void IntroSortInternal(T* First, SIZE_T Num, ProjectionType Projection, PredicateType Predicate);
}

namespace Algo
{
    /**
     * Sort a range of elements using its operator<. The sort is unstable.
     *
     * @param Range	The range to sort.
     */
    template <typename RangeType>
    FORCEINLINE void IntroSort(RangeType& Range)
    {
        AlgoImpl::IntroSortInternal(GetData(Range), GetNum(Range), FIdentityFunctor(), TLess<>());
    }

    /**
     * Sort a range of elements using a user-defined predicate class. The sort is unstable.
     *
     * @param Range		The range to sort.
     * @param Predicate	A binary predicate object used to specify if one element should precede another.
     */
    template <typename RangeType, typename PredicateType>
    FORCEINLINE void IntroSort(RangeType& Range, PredicateType Predicate)
    {
        AlgoImpl::IntroSortInternal(GetData(Range), GetNum(Range), FIdentityFunctor(), MoveTemp(Predicate));
    }

    /**
     * Sort a range of elements by a projection using the projection's operator<. The sort is unstable.
     *
     * @param Range			The range to sort.
     * @param Projection	The projection to sort by when applied to the element.
     */
    template <typename RangeType, typename ProjectionType>
    FORCEINLINE void IntroSortBy(RangeType& Range, ProjectionType Projection)
    {
        AlgoImpl::IntroSortInternal(GetData(Range), GetNum(Range), MoveTemp(Projection), TLess<>());
    }

    /**
     * Sort a range of elements by a projection using a user-defined predicate class. The sort is unstable.
     *
     * @param Range			The range to sort.
     * @param Projection	The projection to sort by when applied to the element.
     * @param Predicate		A binary predicate object, applied to the projection, used to specify if one element should precede another.
     */
    template <typename RangeType, typename ProjectionType, typename PredicateType>
    FORCEINLINE void IntroSortBy(RangeType& Range, ProjectionType Projection, PredicateType Predicate)
    {
        AlgoImpl::IntroSortInternal(GetData(Range), GetNum(Range), MoveTemp(Projection), MoveTemp(Predicate));
    }
}