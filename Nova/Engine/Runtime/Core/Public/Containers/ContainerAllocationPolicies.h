#pragma once


#include "CoreTypes.h"
#include "Misc/AssertionMacros.h"
#include "HAL/UnrealMemory.h"
#include "Templates/TypeCompatibleBytes.h"
#include "HAL/PlatformMath.h"
#include "Math/NumericLimits.h"
#include "Templates/MemoryOps.h"

/** branchless pointer selection
* return A ? A : B;
**/
template<typename ReferencedType>
FORCEINLINE ReferencedType* IfAThenAElseB(ReferencedType* A, ReferencedType* B)
{
    const PTRINT IntA = reinterpret_cast<PTRINT>(A);
    const PTRINT IntB = reinterpret_cast<PTRINT>(B);

    // Compute a mask which has all bits set if IntA is zero, and no bits set if it's non-zero.
    const PTRINT MaskB = -(!IntA);

    return reinterpret_cast<ReferencedType*>(IntA | (MaskB & IntB));
}

/** branchless pointer selection based on predicate
* return PTRINT(Predicate) ? A : B;
**/
template<typename PredicateType, typename ReferencedType>
FORCEINLINE ReferencedType* IfPThenAElseB(PredicateType Predicate, ReferencedType* A, ReferencedType* B)
{
    const PTRINT IntA = reinterpret_cast<PTRINT>(A);
    const PTRINT IntB = reinterpret_cast<PTRINT>(B);

    // Compute a mask which has all bits set if Predicate is zero, and no bits set if it's non-zero.
    const PTRINT MaskB = -(!PTRINT(Predicate));

    return reinterpret_cast<ReferencedType*>((IntA & ~MaskB) | (IntB & MaskB));
}

FORCEINLINE int32 DefaultCalculateSlackShrink(int32 NumElements, int32 NumAllocatedElements, SIZE_T BytesPerElement, bool bAllowQuantize, uint32 Alignment = DEFAULT_ALIGNMENT)
{
    int32 Retval;

    // If the container has too much slack, shrink it to exactly fit the number of elements.
    const uint32 CurrentSlackElements = NumAllocatedElements - NumElements;
    const SIZE_T CurrentSlackBytes = (NumAllocatedElements - NumElements) * BytesPerElement;
    const bool bTooManySlackBytes = CurrentSlackBytes >= 16384;
    const bool bTooManySlackElements = 3 * NumElements < 2 * NumAllocatedElements;
    if ((bTooManySlackBytes || bTooManySlackElements) && (CurrentSlackElements > 64 || !NumElements)) //  hard coded 64 :-(
    {
        Retval = NumElements;
        if (Retval > 0)
        {
            if (bAllowQuantize)
            {
                Retval = (int32)FMemory::QuantizeSize(Retval * BytesPerElement, Alignment) / (int32)BytesPerElement;
            }
        }
    }
    else
    {
        Retval = NumAllocatedElements;
    }

    return Retval;
}

FORCEINLINE int32 DefaultCalculateSlackGrow(int32 NumElements, int32 NumAllocatedElements, SIZE_T BytesPerElement, bool bAllowQuantize, uint32 Alignment = DEFAULT_ALIGNMENT)
{
    int32 Retval;
    checkSlow(NumElements > NumAllocatedElements && NumElements > 0);

    SIZE_T Grow = 4; // this is the amount for the first alloc
    if (NumAllocatedElements || SIZE_T(NumElements) > Grow)
    {
        // Allocate slack for the array proportional to its size.
        Grow = SIZE_T(NumElements) + 3 * SIZE_T(NumElements) / 8 + 16;
    }
    if (bAllowQuantize)
    {
        Retval = (int32)FMemory::QuantizeSize(Grow * BytesPerElement, Alignment) / (int32)BytesPerElement;
    }
    else
    {
        Retval = (int32)Grow;
    }
    // NumElements and MaxElements are stored in 32 bit signed integers so we must be careful not to overflow here.
    if (NumElements > Retval)
    {
        Retval = MAX_int32;
    }

    return Retval;
}

FORCEINLINE int32 DefaultCalculateSlackReserve(int32 NumElements, SIZE_T BytesPerElement, bool bAllowQuantize, uint32 Alignment = DEFAULT_ALIGNMENT)
{
    int32 Retval = NumElements;
    checkSlow(NumElements > 0);
    if (bAllowQuantize)
    {
        Retval = (int32)FMemory::QuantizeSize(SIZE_T(Retval) * SIZE_T(BytesPerElement), Alignment) / (int32)BytesPerElement;
        // NumElements and MaxElements are stored in 32 bit signed integers so we must be careful not to overflow here.
        if (NumElements > Retval)
        {
            Retval = MAX_int32;
        }
    }

    return Retval;
}

/** A type which is used to represent a script type that is unknown at compile time. */
struct FScriptContainerElement
{
};

template <typename AllocatorType>
struct TAllocatorTraitsBase
{
    enum { SupportsMove = false };
    enum { IsZeroConstruct = false };
};

template <typename AllocatorType>
struct TAllocatorTraits : TAllocatorTraitsBase<AllocatorType>
{
};

/** The indirect allocation policy always allocates the elements indirectly. */
class FHeapAllocator
{
public:

    enum { NeedsElementType = false };
    enum { RequireRangeCheck = true };

    class ForAnyElementType
    {
    public:
        /** Default constructor. */
        ForAnyElementType()
            : Data(nullptr)
        {}

        /**
         * Moves the state of another allocator into this one.
         * Assumes that the allocator is currently empty, i.e. memory may be allocated but any existing elements have already been destructed (if necessary).
         * @param Other - The allocator to move the state from.  This allocator should be left in a valid empty state.
         */
        FORCEINLINE void MoveToEmpty(ForAnyElementType& Other)
        {
            //checkSlow(this != &Other);

            if (Data)
            {
                FMemory::Free(Data);
            }

            Data = Other.Data;
            Other.Data = nullptr;
        }

        /** Destructor. */
        FORCEINLINE ~ForAnyElementType()
        {
            if (Data)
            {
                FMemory::Free(Data);
            }
        }

        // FContainerAllocatorInterface
        FORCEINLINE FScriptContainerElement* GetAllocation() const
        {
            return Data;
        }
        FORCEINLINE void ResizeAllocation(int32 PreviousNumElements, int32 NumElements, SIZE_T NumBytesPerElement)
        {
            // Avoid calling FMemory::Realloc( nullptr, 0 ) as ANSI C mandates returning a valid pointer which is not what we want.
            if (Data || NumElements)
            {
                //checkSlow(((uint64)NumElements*(uint64)ElementTypeInfo.GetSize() < (uint64)INT_MAX));
                Data = (FScriptContainerElement*)FMemory::Realloc(Data, NumElements * NumBytesPerElement);
            }
        }
        FORCEINLINE int32 CalculateSlackReserve(int32 NumElements, int32 NumBytesPerElement) const
        {
            return DefaultCalculateSlackReserve(NumElements, NumBytesPerElement, true);
        }
        FORCEINLINE int32 CalculateSlackShrink(int32 NumElements, int32 NumAllocatedElements, int32 NumBytesPerElement) const
        {
            return DefaultCalculateSlackShrink(NumElements, NumAllocatedElements, NumBytesPerElement, true);
        }
        FORCEINLINE int32 CalculateSlackGrow(int32 NumElements, int32 NumAllocatedElements, int32 NumBytesPerElement) const
        {
            return DefaultCalculateSlackGrow(NumElements, NumAllocatedElements, NumBytesPerElement, true);
        }

        SIZE_T GetAllocatedSize(int32 NumAllocatedElements, SIZE_T NumBytesPerElement) const
        {
            return NumAllocatedElements * NumBytesPerElement;
        }

        bool HasAllocation()
        {
            return !!Data;
        }

    private:
        ForAnyElementType(const ForAnyElementType&);
        ForAnyElementType& operator=(const ForAnyElementType&);

        /** A pointer to the container's elements. */
        FScriptContainerElement* Data;
    };

    template<typename ElementType>
    class ForElementType : public ForAnyElementType
    {
    public:

        /** Default constructor. */
        ForElementType()
        {}

        FORCEINLINE ElementType* GetAllocation() const
        {
            return (ElementType*)ForAnyElementType::GetAllocation();
        }
    };
};

template <>
struct TAllocatorTraits<FHeapAllocator> : TAllocatorTraitsBase<FHeapAllocator>
{
    enum { SupportsMove = true };
    enum { IsZeroConstruct = true };
};

class FDefaultAllocator;

/**
 * The inline allocation policy allocates up to a specified number of elements in the same allocation as the container.
 * Any allocation needed beyond that causes all data to be moved into an indirect allocation.
 * It always uses DEFAULT_ALIGNMENT.
 */
template <uint32 NumInlineElements, typename SecondaryAllocator = FDefaultAllocator>
class TInlineAllocator
{
public:

    enum { NeedsElementType = true };
    enum { RequireRangeCheck = true };

    template<typename ElementType>
    class ForElementType
    {
    public:

        /** Default constructor. */
        ForElementType()
        {
        }

        /**
         * Moves the state of another allocator into this one.
         * Assumes that the allocator is currently empty, i.e. memory may be allocated but any existing elements have already been destructed (if necessary).
         * @param Other - The allocator to move the state from.  This allocator should be left in a valid empty state.
         */
        FORCEINLINE void MoveToEmpty(ForElementType& Other)
        {
            checkSlow(this != &Other);

            if (!Other.SecondaryData.GetAllocation())
            {
                // Relocate objects from other inline storage only if it was stored inline in Other
                RelocateConstructItems<ElementType>((void*)InlineData, Other.GetInlineElements(), NumInlineElements);
            }

            // Move secondary storage in any case.
            // This will move secondary storage if it exists but will also handle the case where secondary storage is used in Other but not in *this.
            SecondaryData.MoveToEmpty(Other.SecondaryData);
        }

        // FContainerAllocatorInterface
        FORCEINLINE ElementType* GetAllocation() const
        {
            return IfAThenAElseB<ElementType>(SecondaryData.GetAllocation(), GetInlineElements());
        }

        void ResizeAllocation(int32 PreviousNumElements, int32 NumElements, SIZE_T NumBytesPerElement)
        {
            // Check if the new allocation will fit in the inline data area.
            if (NumElements <= NumInlineElements)
            {
                // If the old allocation wasn't in the inline data area, relocate it into the inline data area.
                if (SecondaryData.GetAllocation())
                {
                    RelocateConstructItems<ElementType>((void*)InlineData, (ElementType*)SecondaryData.GetAllocation(), PreviousNumElements);

                    // Free the old indirect allocation.
                    SecondaryData.ResizeAllocation(0, 0, NumBytesPerElement);
                }
            }
            else
            {
                if (!SecondaryData.GetAllocation())
                {
                    // Allocate new indirect memory for the data.
                    SecondaryData.ResizeAllocation(0, NumElements, NumBytesPerElement);

                    // Move the data out of the inline data area into the new allocation.
                    RelocateConstructItems<ElementType>((void*)SecondaryData.GetAllocation(), GetInlineElements(), PreviousNumElements);
                }
                else
                {
                    // Reallocate the indirect data for the new size.
                    SecondaryData.ResizeAllocation(PreviousNumElements, NumElements, NumBytesPerElement);
                }
            }
        }

        FORCEINLINE int32 CalculateSlackReserve(int32 NumElements, SIZE_T NumBytesPerElement) const
        {
            // If the elements use less space than the inline allocation, only use the inline allocation as slack.
            return NumElements <= NumInlineElements ?
                NumInlineElements :
                SecondaryData.CalculateSlackReserve(NumElements, NumBytesPerElement);
        }
        FORCEINLINE int32 CalculateSlackShrink(int32 NumElements, int32 NumAllocatedElements, int32 NumBytesPerElement) const
        {
            // If the elements use less space than the inline allocation, only use the inline allocation as slack.
            return NumElements <= NumInlineElements ?
                NumInlineElements :
                SecondaryData.CalculateSlackShrink(NumElements, NumAllocatedElements, NumBytesPerElement);
        }
        FORCEINLINE int32 CalculateSlackGrow(int32 NumElements, int32 NumAllocatedElements, int32 NumBytesPerElement) const
        {
            // If the elements use less space than the inline allocation, only use the inline allocation as slack.
            return NumElements <= NumInlineElements ?
                NumInlineElements :
                SecondaryData.CalculateSlackGrow(NumElements, NumAllocatedElements, NumBytesPerElement);
        }

        SIZE_T GetAllocatedSize(int32 NumAllocatedElements, SIZE_T NumBytesPerElement) const
        {
            return SecondaryData.GetAllocatedSize(NumAllocatedElements, NumBytesPerElement);
        }

        bool HasAllocation()
        {
            return SecondaryData.HasAllocation();
        }

    private:
        ForElementType(const ForElementType&);
        ForElementType& operator=(const ForElementType&);

        /** The data is stored in this array if less than NumInlineElements is needed. */
        TTypeCompatibleBytes<ElementType> InlineData[NumInlineElements];

        /** The data is allocated through the indirect allocation policy if more than NumInlineElements is needed. */
        typename SecondaryAllocator::template ForElementType<ElementType> SecondaryData;

        /** @return the base of the aligned inline element data */
        ElementType* GetInlineElements() const
        {
            return (ElementType*)InlineData;
        }
    };

    typedef void ForAnyElementType;
};

template <uint32 NumInlineElements, typename SecondaryAllocator>
struct TAllocatorTraits<TInlineAllocator<NumInlineElements, SecondaryAllocator>> : TAllocatorTraitsBase<TInlineAllocator<NumInlineElements, SecondaryAllocator>>
{
    enum { SupportsMove = TAllocatorTraits<SecondaryAllocator>::SupportsMove };
};

// We want these to be correctly typed as int32, but we don't want them to have linkage, so we make them macros
#define NumBitsPerDWORD ((int32)32)
#define NumBitsPerDWORDLogTwo ((int32)5)

class FDefaultAllocator;
class FDefaultBitArrayAllocator;

/** Encapsulates the allocators used by a sparse array in a single type. */
template<typename InElementAllocator = FDefaultAllocator, typename InBitArrayAllocator = FDefaultBitArrayAllocator>
class TSparseArrayAllocator
{
public:

    typedef InElementAllocator ElementAllocator;
    typedef InBitArrayAllocator BitArrayAllocator;
};

//
// Set allocation definitions.
//

#define DEFAULT_NUMBER_OF_ELEMENTS_PER_HASH_BUCKET	2
#define DEFAULT_BASE_NUMBER_OF_HASH_BUCKETS			8
#define DEFAULT_MIN_NUMBER_OF_HASHED_ELEMENTS		4

/** Encapsulates the allocators used by a set in a single type. */
template<
    typename InSparseArrayAllocator = TSparseArrayAllocator<>,
    typename InHashAllocator = TInlineAllocator<1, FDefaultAllocator>,
    uint32   AverageNumberOfElementsPerHashBucket = DEFAULT_NUMBER_OF_ELEMENTS_PER_HASH_BUCKET,
    uint32   BaseNumberOfHashBuckets = DEFAULT_BASE_NUMBER_OF_HASH_BUCKETS,
    uint32   MinNumberOfHashedElements = DEFAULT_MIN_NUMBER_OF_HASHED_ELEMENTS
>
class TSetAllocator
{
public:

    /** Computes the number of hash buckets to use for a given number of elements. */
    static FORCEINLINE uint32 GetNumberOfHashBuckets(uint32 NumHashedElements)
    {
        if (NumHashedElements >= MinNumberOfHashedElements)
        {
            return FPlatformMath::RoundUpToPowerOfTwo(NumHashedElements / AverageNumberOfElementsPerHashBucket + BaseNumberOfHashBuckets);
        }

        return 1;
    }

    typedef InSparseArrayAllocator SparseArrayAllocator;
    typedef InHashAllocator        HashAllocator;
};

class FDefaultAllocator : public FHeapAllocator { public: typedef FHeapAllocator          Typedef; };
class FDefaultSetAllocator : public TSetAllocator<> { public: typedef TSetAllocator<>         Typedef; };
class FDefaultBitArrayAllocator : public TInlineAllocator<4> { public: typedef TInlineAllocator<4>     Typedef; };
class FDefaultSparseArrayAllocator : public TSparseArrayAllocator<> { public: typedef TSparseArrayAllocator<> Typedef; };

template <> struct TAllocatorTraits<FDefaultAllocator> : TAllocatorTraits<typename FDefaultAllocator::Typedef> {};
template <> struct TAllocatorTraits<FDefaultSetAllocator> : TAllocatorTraits<typename FDefaultSetAllocator::Typedef> {};
template <> struct TAllocatorTraits<FDefaultBitArrayAllocator> : TAllocatorTraits<typename FDefaultBitArrayAllocator::Typedef> {};
template <> struct TAllocatorTraits<FDefaultSparseArrayAllocator> : TAllocatorTraits<typename FDefaultSparseArrayAllocator::Typedef> {};