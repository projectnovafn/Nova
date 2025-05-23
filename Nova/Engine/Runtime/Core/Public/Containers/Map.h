#pragma once
#include "Set.h"
#include "Templates/HasGetTypeHash.h"
#include "Templates/Sorting.h"
#include "Templates/UnrealTemplate.h"


#define ExchangeB(A,B) {bool T=A; A=B; B=T;}

// TODO proper tuple implementations
template<typename KeyType, typename ValueType>
class TPair
{
public:
    KeyType   Key;
    ValueType Value;
};

template <typename KeyInitType, typename ValueInitType>
class TPairInitializer
{
public:
    typename TRValueToLValueReference<KeyInitType  >::Type Key;
    typename TRValueToLValueReference<ValueInitType>::Type Value;

    /** Initialization constructor. */
    FORCEINLINE TPairInitializer(KeyInitType InKey, ValueInitType InValue)
        : Key(InKey)
        , Value(InValue)
    {
    }

    /** Implicit conversion to pair initializer. */
    template <typename KeyType, typename ValueType>
    FORCEINLINE TPairInitializer(const TPair<KeyType, ValueType>& Pair)
        : Key(Pair.Key)
        , Value(Pair.Value)
    {
    }

    template <typename KeyType, typename ValueType>
    operator TPair<KeyType, ValueType>() const
    {
        return TPair<KeyType, ValueType>(StaticCast<KeyInitType>(Key), StaticCast<ValueInitType>(Value));
    }
};

/** An initializer type for keys that's passed to the pair set when adding a new key. */
template <typename KeyInitType>
class TKeyInitializer
{
public:
    typename TRValueToLValueReference<KeyInitType>::Type Key;

    /** Initialization constructor. */
    FORCEINLINE explicit TKeyInitializer(KeyInitType InKey)
        : Key(InKey)
    { }

    template <typename KeyType, typename ValueType>
    operator TPair<KeyType, ValueType>() const
    {
        return TPair<KeyType, ValueType>(StaticCast<KeyInitType>(Key), ValueType());
    }
};

/** Defines how the map's pairs are hashed. */
template<typename KeyType, typename ValueType, bool bInAllowDuplicateKeys>
struct TDefaultMapKeyFuncs : BaseKeyFuncs<TPair<KeyType, ValueType>, KeyType, bInAllowDuplicateKeys>
{
    typedef typename TTypeTraits<KeyType>::ConstPointerType KeyInitType;
    typedef const TPairInitializer<typename TTypeTraits<KeyType>::ConstInitType, typename TTypeTraits<ValueType>::ConstInitType>& ElementInitType;

    static FORCEINLINE KeyInitType GetSetKey(ElementInitType Element)
    {
        return Element.Key;
    }
    static FORCEINLINE bool Matches(KeyInitType A, KeyInitType B)
    {
        return A == B;
    }
    static FORCEINLINE uint32 GetKeyHash(KeyInitType Key)
    {
        return GetTypeHash(Key);
    }
};

template<typename KeyType, typename ValueType, bool bInAllowDuplicateKeys>
struct TDefaultMapHashableKeyFuncs : TDefaultMapKeyFuncs<KeyType, ValueType, bInAllowDuplicateKeys>
{
    static_assert(THasGetTypeHash<KeyType>::Value, "TMap must have a hashable KeyType unless a custom key func is provided.");
};

/**
 * The base class of maps from keys to values.  Implemented using a TSet of key-value pairs with a custom KeyFuncs,
 * with the same O(1) addition, removal, and finding.
 **/
template <typename KeyType, typename ValueType, typename SetAllocator, typename KeyFuncs>
class TMapBase
{
    template <typename OtherKeyType, typename OtherValueType, typename OtherSetAllocator, typename OtherKeyFuncs>
    friend class TMapBase;

    friend struct TContainerTraits<TMapBase>;

public:
    typedef typename TTypeTraits<KeyType  >::ConstPointerType KeyConstPointerType;
    typedef typename TTypeTraits<KeyType  >::ConstInitType    KeyInitType;
    typedef typename TTypeTraits<ValueType>::ConstInitType    ValueInitType;
    typedef TPair<KeyType, ValueType> ElementType;

protected:
    TMapBase() = default;
    TMapBase(TMapBase&&) = default;
    TMapBase(const TMapBase&) = default;
    TMapBase& operator=(TMapBase&&) = default;
    TMapBase& operator=(const TMapBase&) = default;

    /** Constructor for moving elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TMapBase(TMapBase<KeyType, ValueType, OtherSetAllocator, KeyFuncs>&& Other)
        : Pairs(MoveTemp(Other.Pairs))
    { }

    /** Constructor for copying elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TMapBase(const TMapBase<KeyType, ValueType, OtherSetAllocator, KeyFuncs>& Other)
        : Pairs(Other.Pairs)
    { }

    /** Assignment operator for moving elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TMapBase& operator=(TMapBase<KeyType, ValueType, OtherSetAllocator, KeyFuncs>&& Other)
    {
        Pairs = MoveTemp(Other.Pairs);
        return *this;
    }

    /** Assignment operator for copying elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TMapBase& operator=(const TMapBase<KeyType, ValueType, OtherSetAllocator, KeyFuncs>& Other)
    {
        Pairs = Other.Pairs;
        return *this;
    }

public:
    // Legacy comparison operators.  Note that these also test whether the map's key-value pairs were added in the same order!
    friend bool LegacyCompareEqual(const TMapBase& A, const TMapBase& B)
    {
        return LegacyCompareEqual(A.Pairs, B.Pairs);
    }
    friend bool LegacyCompareNotEqual(const TMapBase& A, const TMapBase& B)
    {
        return LegacyCompareNotEqual(A.Pairs, B.Pairs);
    }

    /**
     * Compare this map with another for equality. Does not make any assumptions about Key order.
     * NOTE: this might be a candidate for operator== but it was decided to make it an explicit function
     *  since it can potentially be quite slow.
     *
     * @param Other The other map to compare against
     * @returns True if both this and Other contain the same keys with values that compare ==
     */
    bool OrderIndependentCompareEqual(const TMapBase& Other) const
    {
        // first check counts (they should be the same obviously)
        if (Num() != Other.Num())
        {
            return false;
        }

        // since we know the counts are the same, we can just iterate one map and check for existence in the other
        for (typename ElementSetType::TConstIterator It(Pairs); It; ++It)
        {
            const ValueType* BVal = Other.Find(It->Key);
            if (BVal == nullptr)
            {
                return false;
            }
            if (!(*BVal == It->Value))
            {
                return false;
            }
        }

        // all fields in A match B and A and B's counts are the same (so there can be no fields in B not in A)
        return true;
    }

    /**
     * Removes all elements from the map.
     *
     * This method potentially leaves space allocated for an expected
     * number of elements about to be added.
     *
     * @param ExpectedNumElements The number of elements about to be added to the set.
     */
    FORCEINLINE void Empty(int32 ExpectedNumElements = 0)
    {
        Pairs.Empty(ExpectedNumElements);
    }

    /** Efficiently empties out the map but preserves all allocations and capacities */
    FORCEINLINE void Reset()
    {
        Pairs.Reset();
    }

    /** Shrinks the pair set to avoid slack. */
    FORCEINLINE void Shrink()
    {
        Pairs.Shrink();
    }

    /** Compacts the pair set to remove holes */
    FORCEINLINE void Compact()
    {
        Pairs.Compact();
    }

    /** Compacts the pair set to remove holes. Does not change the iteration order of the elements. */
    FORCEINLINE void CompactStable()
    {
        Pairs.CompactStable();
    }

    /** Preallocates enough memory to contain Number elements */
    FORCEINLINE void Reserve(int32 Number)
    {
        Pairs.Reserve(Number);
    }

    /** @return The number of elements in the map. */
    FORCEINLINE int32 Num() const
    {
        return Pairs.Num();
    }

    /**
     * Get the unique keys contained within this map.
     *
     * @param OutKeys Upon return, contains the set of unique keys in this map.
     * @return The number of unique keys in the map.
     */
    template<typename Allocator> int32 GetKeys(TArray<KeyType, Allocator>& OutKeys) const
    {
        TSet<KeyType> VisitedKeys;
        for (typename ElementSetType::TConstIterator It(Pairs); It; ++It)
        {
            if (!VisitedKeys.Contains(It->Key))
            {
                OutKeys.Add(It->Key);
                VisitedKeys.Add(It->Key);
            }
        }
        return OutKeys.Num();
    }

    /**
     * Helper function to return the amount of memory allocated by this container .
     * Only returns the size of allocations made directly by the container, not the elements themselves.
     *
     * @return Number of bytes allocated by this container.
     * @see CountBytes
     */
    FORCEINLINE uint32 GetAllocatedSize() const
    {
        return Pairs.GetAllocatedSize();
    }

    /**
     * Set the value associated with a key.
     *
     * @param InKey The key to associate the value with.
     * @param InValue The value to associate with the key.
     * @return A reference to the value as stored in the map. The reference is only valid until the next change to any key in the map.
     */
    FORCEINLINE ValueType& Add(const KeyType& InKey, const ValueType& InValue) { return Emplace(InKey, InValue); }
    FORCEINLINE ValueType& Add(const KeyType& InKey, ValueType&& InValue) { return Emplace(InKey, MoveTempIfPossible(InValue)); }
    FORCEINLINE ValueType& Add(KeyType&& InKey, const ValueType& InValue) { return Emplace(MoveTempIfPossible(InKey), InValue); }
    FORCEINLINE ValueType& Add(KeyType&& InKey, ValueType&& InValue) { return Emplace(MoveTempIfPossible(InKey), MoveTempIfPossible(InValue)); }

    /**
     * Set a default value associated with a key.
     *
     * @param InKey The key to associate the value with.
     * @return A reference to the value as stored in the map. The reference is only valid until the next change to any key in the map.
     */
    FORCEINLINE ValueType& Add(const KeyType& InKey) { return Emplace(InKey); }
    FORCEINLINE ValueType& Add(KeyType&& InKey) { return Emplace(MoveTempIfPossible(InKey)); }

    /**
     * Sets the value associated with a key.
     *
     * @param InKey The key to associate the value with.
     * @param InValue The value to associate with the key.
     * @return A reference to the value as stored in the map. The reference is only valid until the next change to any key in the map.	 */
    template <typename InitKeyType, typename InitValueType>
    ValueType& Emplace(InitKeyType&& InKey, InitValueType&& InValue)
    {
        const FSetElementId PairId = Pairs.Emplace(TPairInitializer<InitKeyType&&, InitValueType&&>(Forward<InitKeyType>(InKey), Forward<InitValueType>(InValue)));

        return Pairs[PairId].Value;
    }

    /**
     * Set a default value associated with a key.
     *
     * @param InKey The key to associate the value with.
     * @return A reference to the value as stored in the map. The reference is only valid until the next change to any key in the map.
     */
    template <typename InitKeyType>
    ValueType& Emplace(InitKeyType&& InKey)
    {
        const FSetElementId PairId = Pairs.Emplace(TKeyInitializer<InitKeyType&&>(Forward<InitKeyType>(InKey)));

        return Pairs[PairId].Value;
    }

    /**
     * Remove all value associations for a key.
     *
     * @param InKey The key to remove associated values for.
     * @return The number of values that were associated with the key.
     */
    FORCEINLINE int32 Remove(KeyConstPointerType InKey)
    {
        const int32 NumRemovedPairs = Pairs.Remove(InKey);
        return NumRemovedPairs;
    }

    /**
     * Find the key associated with the specified value.
     *
     * The time taken is O(N) in the number of pairs.
     *
     * @param Value The value to search for
     * @return A pointer to the key associated with the specified value,
     *     or nullptr if the value isn't contained in this map. The pointer
     *     is only valid until the next change to any key in the map.
     */
    const KeyType* FindKey(ValueInitType Value) const
    {
        for (typename ElementSetType::TConstIterator PairIt(Pairs); PairIt; ++PairIt)
        {
            if (PairIt->Value == Value)
            {
                return &PairIt->Key;
            }
        }
        return nullptr;
    }

    /**
     * Find the value associated with a specified key.
     *
     * @param Key The key to search for.
     * @return A pointer to the value associated with the specified key, or nullptr if the key isn't contained in this map.  The pointer
     *			is only valid until the next change to any key in the map.
     */
    FORCEINLINE ValueType* Find(KeyConstPointerType Key)
    {
        if (auto* Pair = Pairs.Find(Key))
        {
            return &Pair->Value;
        }

        return nullptr;
    }
    FORCEINLINE const ValueType* Find(KeyConstPointerType Key) const
    {
        return const_cast<TMapBase*>(this)->Find(Key);
    }

private:

    /**
     * Find the value associated with a specified key, or if none exists,
     * adds a value using the default constructor.
     *
     * @param Key The key to search for.
     * @return A reference to the value associated with the specified key.
     */
    template <typename ArgType>
    FORCEINLINE ValueType& FindOrAddImpl(ArgType&& Arg)
    {
        if (auto* Pair = Pairs.Find(Arg))
            return Pair->Value;

        return Add(Forward<ArgType>(Arg));
    }

public:

    /**
     * Find the value associated with a specified key, or if none exists,
     * adds a value using the default constructor.
     *
     * @param Key The key to search for.
     * @return A reference to the value associated with the specified key.
     */
    FORCEINLINE ValueType& FindOrAdd(const KeyType& Key) { return FindOrAddImpl(Key); }
    FORCEINLINE ValueType& FindOrAdd(KeyType&& Key) { return FindOrAddImpl(MoveTempIfPossible(Key)); }

    /**
     * Find the value associated with a specified key, or if none exists,
     * adds a value using the key as the constructor parameter.
     *
     * @param Key The key to search for.
     * @return A reference to the value associated with the specified key.
     */
     /*ValueType& FindOrAddKey(KeyInitType Key)
     {
         TPair* Pair = Pairs.Find(Key);
         if( Pair )
         {
             return Pair->Value;
         }
         else
         {
             return Set(Key, ValueType(Key));
         }
     }*/

     /**
      * Find a reference to the value associated with a specified key.
      *
      * @param Key The key to search for.
      * @return The value associated with the specified key, or triggers an assertion if the key does not exist.
      */
    FORCEINLINE const ValueType& FindChecked(KeyConstPointerType Key) const
    {
        const auto* Pair = Pairs.Find(Key);
        check(Pair != nullptr);
        return Pair->Value;
    }

    /**
     * Find a reference to the value associated with a specified key.
     *
     * @param Key The key to search for.
     * @return The value associated with the specified key, or triggers an assertion if the key does not exist.
     */
    FORCEINLINE ValueType& FindChecked(KeyConstPointerType Key)
    {
        auto* Pair = Pairs.Find(Key);
        check(Pair != nullptr);
        return Pair->Value;
    }

    /**
     * Find the value associated with a specified key.
     *
     * @param Key The key to search for.
     * @return The value associated with the specified key, or the default value for the ValueType if the key isn't contained in this map.
     */
    FORCEINLINE ValueType FindRef(KeyConstPointerType Key) const
    {
        if (const auto* Pair = Pairs.Find(Key))
        {
            return Pair->Value;
        }

        return ValueType();
    }

    /**
     * Check if map contains the specified key.
     *
     * @param Key The key to check for.
     * @return true if the map contains the key.
     */
    FORCEINLINE bool Contains(KeyConstPointerType Key) const
    {
        return Pairs.Contains(Key);
    }

    /**
     * Generate an array from the keys in this map.
     *
     * @param OutArray Will contain the collection of keys.
     */
    template<typename Allocator> void GenerateKeyArray(TArray<KeyType, Allocator>& OutArray) const
    {
        OutArray.Empty(Pairs.Num());
        for (typename ElementSetType::TConstIterator PairIt(Pairs); PairIt; ++PairIt)
        {
            new(OutArray) KeyType(PairIt->Key);
        }
    }

    /**
     * Generate an array from the values in this map.
     *
     * @param OutArray Will contain the collection of values.
     */
    template<typename Allocator> void GenerateValueArray(TArray<ValueType, Allocator>& OutArray) const
    {
        OutArray.Empty(Pairs.Num());
        for (typename ElementSetType::TConstIterator PairIt(Pairs); PairIt; ++PairIt)
        {
            new(OutArray) ValueType(PairIt->Value);
        }
    }

protected:
    typedef TSet<ElementType, KeyFuncs, SetAllocator> ElementSetType;

    /** The base of TMapBase iterators. */
    template<bool bConst, bool bRangedFor = false>
    class TBaseIterator
    {
    public:
        typedef typename TChooseClass<
            bConst,
            typename TChooseClass<bRangedFor, typename ElementSetType::TRangedForConstIterator, typename ElementSetType::TConstIterator>::Result,
            typename TChooseClass<bRangedFor, typename ElementSetType::TRangedForIterator, typename ElementSetType::TIterator     >::Result
        >::Result PairItType;
    private:
        typedef typename TChooseClass<bConst, const TMapBase, TMapBase>::Result MapType;
        typedef typename TChooseClass<bConst, const KeyType, KeyType>::Result ItKeyType;
        typedef typename TChooseClass<bConst, const ValueType, ValueType>::Result ItValueType;
        typedef typename TChooseClass<bConst, const typename ElementSetType::ElementType, typename ElementSetType::ElementType>::Result PairType;

    public:
        FORCEINLINE TBaseIterator(const PairItType& InElementIt)
            : PairIt(InElementIt)
        {
        }

        FORCEINLINE TBaseIterator& operator++()
        {
            ++PairIt;
            return *this;
        }

        /** conversion to "bool" returning true if the iterator is valid. */
        FORCEINLINE explicit operator bool() const
        {
            return !!PairIt;
        }
        /** inverse of the "bool" operator */
        FORCEINLINE bool operator !() const
        {
            return !(bool)*this;
        }

        FORCEINLINE friend bool operator==(const TBaseIterator& Lhs, const TBaseIterator& Rhs) { return Lhs.PairIt == Rhs.PairIt; }
        FORCEINLINE friend bool operator!=(const TBaseIterator& Lhs, const TBaseIterator& Rhs) { return Lhs.PairIt != Rhs.PairIt; }

        FORCEINLINE ItKeyType& Key()   const { return PairIt->Key; }
        FORCEINLINE ItValueType& Value() const { return PairIt->Value; }

        FORCEINLINE PairType& operator* () const { return  *PairIt; }
        FORCEINLINE PairType* operator->() const { return &*PairIt; }

    protected:
        PairItType PairIt;
    };

    /** The base type of iterators that iterate over the values associated with a specified key. */
    template<bool bConst>
    class TBaseKeyIterator
    {
    private:
        typedef typename TChooseClass<bConst, typename ElementSetType::TConstKeyIterator, typename ElementSetType::TKeyIterator>::Result SetItType;
        typedef typename TChooseClass<bConst, const KeyType, KeyType>::Result ItKeyType;
        typedef typename TChooseClass<bConst, const ValueType, ValueType>::Result ItValueType;

    public:
        /** Initialization constructor. */
        FORCEINLINE TBaseKeyIterator(const SetItType& InSetIt)
            : SetIt(InSetIt)
        {
        }

        FORCEINLINE TBaseKeyIterator& operator++()
        {
            ++SetIt;
            return *this;
        }

        /** conversion to "bool" returning true if the iterator is valid. */
        FORCEINLINE explicit operator bool() const
        {
            return !!SetIt;
        }
        /** inverse of the "bool" operator */
        FORCEINLINE bool operator !() const
        {
            return !(bool)*this;
        }

        FORCEINLINE ItKeyType& Key() const { return SetIt->Key; }
        FORCEINLINE ItValueType& Value() const { return SetIt->Value; }

    protected:
        SetItType SetIt;
    };

    /** A set of the key-value pairs in the map. */
    ElementSetType Pairs;

public:

    /** Map iterator. */
    class TIterator : public TBaseIterator<false>
    {
    public:

        /** Initialization constructor. */
        FORCEINLINE TIterator(TMapBase& InMap, bool bInRequiresRehashOnRemoval = false)
            : TBaseIterator<false>(InMap.Pairs.CreateIterator())
            , Map(InMap)
            , bElementsHaveBeenRemoved(false)
            , bRequiresRehashOnRemoval(bInRequiresRehashOnRemoval)
        {
        }

        /** Destructor. */
        FORCEINLINE ~TIterator()
        {
            if (bElementsHaveBeenRemoved && bRequiresRehashOnRemoval)
            {
                Map.Pairs.Relax();
            }
        }

        /** Removes the current pair from the map. */
        FORCEINLINE void RemoveCurrent()
        {
            TBaseIterator<false>::PairIt.RemoveCurrent();
            bElementsHaveBeenRemoved = true;
        }

    private:
        TMapBase& Map;
        bool      bElementsHaveBeenRemoved;
        bool      bRequiresRehashOnRemoval;
    };

    /** Const map iterator. */
    class TConstIterator : public TBaseIterator<true>
    {
    public:
        FORCEINLINE TConstIterator(const TMapBase& InMap)
            : TBaseIterator<true>(InMap.Pairs.CreateConstIterator())
        {
        }
    };

    using TRangedForIterator = TBaseIterator<false, true>;
    using TRangedForConstIterator = TBaseIterator<true, true>;

    /** Iterates over values associated with a specified key in a const map. */
    class TConstKeyIterator : public TBaseKeyIterator<true>
    {
    public:
        FORCEINLINE TConstKeyIterator(const TMapBase& InMap, KeyInitType InKey)
            : TBaseKeyIterator<true>(typename ElementSetType::TConstKeyIterator(InMap.Pairs, InKey))
        {}
    };

    /** Iterates over values associated with a specified key in a map. */
    class TKeyIterator : public TBaseKeyIterator<false>
    {
    public:
        FORCEINLINE TKeyIterator(TMapBase& InMap, KeyInitType InKey)
            : TBaseKeyIterator<false>(typename ElementSetType::TKeyIterator(InMap.Pairs, InKey))
        {}

        /** Removes the current key-value pair from the map. */
        FORCEINLINE void RemoveCurrent()
        {
            TBaseKeyIterator<false>::SetIt.RemoveCurrent();
        }
    };

    /** Creates an iterator over all the pairs in this map */
    FORCEINLINE TIterator CreateIterator()
    {
        return TIterator(*this);
    }

    /** Creates a const iterator over all the pairs in this map */
    FORCEINLINE TConstIterator CreateConstIterator() const
    {
        return TConstIterator(*this);
    }

    /** Creates an iterator over the values associated with a specified key in a map */
    FORCEINLINE TKeyIterator CreateKeyIterator(KeyInitType InKey)
    {
        return TKeyIterator(*this, InKey);
    }

    /** Creates a const iterator over the values associated with a specified key in a map */
    FORCEINLINE TConstKeyIterator CreateConstKeyIterator(KeyInitType InKey) const
    {
        return TConstKeyIterator(*this, InKey);
    }

private:
    /**
     * DO NOT USE DIRECTLY
     * STL-like iterators to enable range-based for loop support.
     */
    FORCEINLINE friend TRangedForIterator      begin(TMapBase& MapBase) { return TRangedForIterator(begin(MapBase.Pairs)); }
    FORCEINLINE friend TRangedForConstIterator begin(const TMapBase& MapBase) { return TRangedForConstIterator(begin(MapBase.Pairs)); }
    FORCEINLINE friend TRangedForIterator      end(TMapBase& MapBase) { return TRangedForIterator(end(MapBase.Pairs)); }
    FORCEINLINE friend TRangedForConstIterator end(const TMapBase& MapBase) { return TRangedForConstIterator(end(MapBase.Pairs)); }
};

/** The base type of sortable maps. */
template <typename KeyType, typename ValueType, typename SetAllocator, typename KeyFuncs>
class TSortableMapBase : public TMapBase<KeyType, ValueType, SetAllocator, KeyFuncs>
{
    friend struct TContainerTraits<TSortableMapBase>;

protected:
    typedef TMapBase<KeyType, ValueType, SetAllocator, KeyFuncs> Super;

    TSortableMapBase() = default;
    TSortableMapBase(TSortableMapBase&&) = default;
    TSortableMapBase(const TSortableMapBase&) = default;
    TSortableMapBase& operator=(TSortableMapBase&&) = default;
    TSortableMapBase& operator=(const TSortableMapBase&) = default;

    /** Constructor for moving elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TSortableMapBase(TSortableMapBase<KeyType, ValueType, OtherSetAllocator, KeyFuncs>&& Other)
        : Super(MoveTemp(Other))
    {
    }

    /** Constructor for copying elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TSortableMapBase(const TSortableMapBase<KeyType, ValueType, OtherSetAllocator, KeyFuncs>& Other)
        : Super(Other)
    {
    }

    /** Assignment operator for moving elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TSortableMapBase& operator=(TSortableMapBase<KeyType, ValueType, OtherSetAllocator, KeyFuncs>&& Other)
    {
        (Super&)*this = MoveTemp(Other);
        return *this;
    }

    /** Assignment operator for copying elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TSortableMapBase& operator=(const TSortableMapBase<KeyType, ValueType, OtherSetAllocator, KeyFuncs>& Other)
    {
        (Super&)*this = Other;
        return *this;
    }

public:
    /**
     * Sorts the pairs array using each pair's Key as the sort criteria, then rebuilds the map's hash.
     * Invoked using "MyMapVar.KeySort( PREDICATE_CLASS() );"
     */
    template<typename PREDICATE_CLASS>
    FORCEINLINE void KeySort(const PREDICATE_CLASS& Predicate)
    {
        Super::Pairs.Sort(FKeyComparisonClass<PREDICATE_CLASS>(Predicate));
    }

    /**
     * Sorts the pairs array using each pair's Value as the sort criteria, then rebuilds the map's hash.
     * Invoked using "MyMapVar.ValueSort( PREDICATE_CLASS() );"
     */
    template<typename PREDICATE_CLASS>
    FORCEINLINE void ValueSort(const PREDICATE_CLASS& Predicate)
    {
        Super::Pairs.Sort(FValueComparisonClass<PREDICATE_CLASS>(Predicate));
    }

private:

    /** Extracts the pair's key from the map's pair structure and passes it to the user provided comparison class. */
    template<typename PREDICATE_CLASS>
    class FKeyComparisonClass
    {
        TDereferenceWrapper< KeyType, PREDICATE_CLASS> Predicate;

    public:

        FORCEINLINE FKeyComparisonClass(const PREDICATE_CLASS& InPredicate)
            : Predicate(InPredicate)
        {}

        FORCEINLINE bool operator()(const typename Super::ElementType& A, const typename Super::ElementType& B) const
        {
            return Predicate(A.Key, B.Key);
        }
    };

    /** Extracts the pair's value from the map's pair structure and passes it to the user provided comparison class. */
    template<typename PREDICATE_CLASS>
    class FValueComparisonClass
    {
        TDereferenceWrapper< ValueType, PREDICATE_CLASS> Predicate;

    public:

        FORCEINLINE FValueComparisonClass(const PREDICATE_CLASS& InPredicate)
            : Predicate(InPredicate)
        {}

        FORCEINLINE bool operator()(const typename Super::ElementType& A, const typename Super::ElementType& B) const
        {
            return Predicate(A.Value, B.Value);
        }
    };
};

class FScriptMap;

/** A TMapBase specialization that only allows a single value associated with each key.*/
template<typename KeyType, typename ValueType, typename SetAllocator, typename KeyFuncs>
class TMap : public TSortableMapBase<KeyType, ValueType, SetAllocator, KeyFuncs>
{
    friend struct TContainerTraits<TMap>;
    friend class  FScriptMap;

    static_assert(!KeyFuncs::bAllowDuplicateKeys, "TMap cannot be instantiated with a KeyFuncs which allows duplicate keys");

public:
    typedef TSortableMapBase<KeyType, ValueType, SetAllocator, KeyFuncs> Super;
    typedef typename Super::KeyInitType KeyInitType;
    typedef typename Super::KeyConstPointerType KeyConstPointerType;

    TMap() = default;
    TMap(TMap&&) = default;
    TMap(const TMap&) = default;
    TMap& operator=(TMap&&) = default;
    TMap& operator=(const TMap&) = default;

    /** Constructor for moving elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TMap(TMap<KeyType, ValueType, OtherSetAllocator, KeyFuncs>&& Other)
        : Super(MoveTemp(Other))
    {
    }

    /** Constructor for copying elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TMap(const TMap<KeyType, ValueType, OtherSetAllocator, KeyFuncs>& Other)
        : Super(Other)
    {
    }

    /** Constructor which gets its elements from a native initializer list */
    TMap(std::initializer_list<TPairInitializer<const KeyType&, const ValueType&>> InitList)
    {
        this->Reserve((int32)InitList.size());
        for (const TPairInitializer<const KeyType&, const ValueType&>& Element : InitList)
        {
            this->Add(Element.Key, Element.Value);
        }
    }

    /** Assignment operator for moving elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TMap& operator=(TMap<KeyType, ValueType, OtherSetAllocator, KeyFuncs>&& Other)
    {
        (Super&)*this = MoveTemp(Other);
        return *this;
    }

    /** Assignment operator for copying elements from a TMap with a different SetAllocator */
    template<typename OtherSetAllocator>
    TMap& operator=(const TMap<KeyType, ValueType, OtherSetAllocator, KeyFuncs>& Other)
    {
        (Super&)*this = Other;
        return *this;
    }

    /** Assignment operator which gets its elements from a native initializer list */
    TMap& operator=(std::initializer_list<TPairInitializer<const KeyType&, const ValueType&>> InitList)
    {
        this->Empty((int32)InitList.size());
        for (const TPairInitializer<const KeyType&, const ValueType&>& Element : InitList)
        {
            this->Add(Element.Key, Element.Value);
        }
        return *this;
    }

    /**
     * Remove the pair with the specified key and copies the value
     * that was removed to the ref parameter
     *
     * @param Key The key to search for
     * @param OutRemovedValue If found, the value that was removed (not modified if the key was not found)
     * @return whether or not the key was found
     */
    FORCEINLINE bool RemoveAndCopyValue(KeyInitType Key, ValueType& OutRemovedValue)
    {
        const FSetElementId PairId = Super::Pairs.FindId(Key);
        if (!PairId.IsValidId())
            return false;

        OutRemovedValue = MoveTempIfPossible(Super::Pairs[PairId].Value);
        Super::Pairs.Remove(PairId);
        return true;
    }

    /**
     * Find a pair with the specified key, removes it from the map, and returns the value part of the pair.
     *
     * If no pair was found, an exception is thrown.
     *
     * @param Key the key to search for
     * @return whether or not the key was found
     */
    FORCEINLINE ValueType FindAndRemoveChecked(KeyConstPointerType Key)
    {
        const FSetElementId PairId = Super::Pairs.FindId(Key);
        check(PairId.IsValidId());
        ValueType Result = MoveTempIfPossible(Super::Pairs[PairId].Value);
        Super::Pairs.Remove(PairId);
        return Result;
    }

    /**
     * Move all items from another map into our map (if any keys are in both,
     * the value from the other map wins) and empty the other map.
     *
     * @param OtherMap The other map of items to move the elements from.
     */
    template<typename OtherSetAllocator>
    void Append(TMap<KeyType, ValueType, OtherSetAllocator, KeyFuncs>&& OtherMap)
    {
        this->Reserve(this->Num() + OtherMap.Num());
        for (auto& Pair : OtherMap)
        {
            this->Add(MoveTempIfPossible(Pair.Key), MoveTempIfPossible(Pair.Value));
        }

        OtherMap.Reset();
    }

    /**
     * Add all items from another map to our map (if any keys are in both,
     * the value from the other map wins).
     *
     * @param OtherMap The other map of items to add.
     */
    template<typename OtherSetAllocator>
    void Append(const TMap<KeyType, ValueType, OtherSetAllocator, KeyFuncs>& OtherMap)
    {
        this->Reserve(this->Num() + OtherMap.Num());
        for (auto& Pair : OtherMap)
        {
            this->Add(Pair.Key, Pair.Value);
        }
    }

    FORCEINLINE       ValueType& operator[](KeyConstPointerType Key) { return this->FindChecked(Key); }
    FORCEINLINE const ValueType& operator[](KeyConstPointerType Key) const { return this->FindChecked(Key); }
};