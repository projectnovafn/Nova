#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Templates/Casts.h"
#include "UObject/PersistentObjectPtr.h"
#include "UObject/SoftObjectPath.h"

/**
 * FSoftObjectPtr is a type of weak pointer to a UObject, that also keeps track of the path to the object on disk.
 * It will change back and forth between being Valid and Pending as the referenced object loads or unloads.
 * It has no impact on if the object is garbage collected or not.
 *
 * This is useful to specify assets that you may want to asynchronously load on demand.
 */
struct FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
{
public:

	/** Returns the StringObjectPath that is wrapped by this SoftObjectPtr */
	FORCEINLINE const FSoftObjectPath& ToSoftObjectPath() const
	{
		return ObjectID;
	}

	/** Returns string representation of reference, in form /package/path.assetname */
	FORCEINLINE FString ToString() const
	{
		return ToSoftObjectPath().ToString();
	}

	/** Returns /package/path string, leaving off the asset name */
	FORCEINLINE FString GetLongPackageName() const
	{
		return ToSoftObjectPath().GetLongPackageName();
	}

	/** Returns assetname string, leaving off the /package/path. part */
	FORCEINLINE FString GetAssetName() const
	{
		return ToSoftObjectPath().GetAssetName();
	}

	FORCEINLINE UObject* Get() const
	{
		return LoadObject<UObject>(*ToString());
	}

	using TPersistentObjectPtr<FSoftObjectPath>::operator=;
};

template <> struct TIsPODType<FSoftObjectPtr> { enum { Value = TIsPODType<TPersistentObjectPtr<FSoftObjectPath> >::Value }; };
template <> struct TIsWeakPointerType<FSoftObjectPtr> { enum { Value = TIsWeakPointerType<TPersistentObjectPtr<FSoftObjectPath> >::Value }; };
template <> struct THasGetTypeHash<FSoftObjectPtr> { enum { Value = THasGetTypeHash<TPersistentObjectPtr<FSoftObjectPath> >::Value }; };

/**
 * TSoftObjectPtr is templatized wrapper of the generic FSoftObjectPtr, it can be used in UProperties
 */
template<class T = UObject>
struct TSoftObjectPtr
{
	template <class U>
	friend struct TSoftObjectPtr;

public:
	/** Default constructor, will be null */
	FORCEINLINE TSoftObjectPtr()
	{
	}

	/** Construct from another soft pointer */
	template <class U, class = typename TEnableIf<TPointerIsConvertibleFromTo<U, T>::Value>::Type>
	FORCEINLINE TSoftObjectPtr(const TSoftObjectPtr<U>& Other)
		: SoftObjectPtr(Other.SoftObjectPtr)
	{
	}

	/** Construct from a moveable soft pointer */
	template <class U, class = typename TEnableIf<TPointerIsConvertibleFromTo<U, T>::Value>::Type>
	FORCEINLINE TSoftObjectPtr(TSoftObjectPtr<U>&& Other)
		: SoftObjectPtr(MoveTemp(Other.SoftObjectPtr))
	{
	}

	/** Construct from an object already in memory */
	template <typename U>
	FORCEINLINE TSoftObjectPtr(const U* Object)
		: SoftObjectPtr(Object)
	{
	}

	/** Construct from a nullptr */
	FORCEINLINE TSoftObjectPtr(TYPE_OF_NULLPTR)
		: SoftObjectPtr(nullptr)
	{
	}

	/** Construct from a soft object path */
	explicit FORCEINLINE TSoftObjectPtr(FSoftObjectPath ObjectPath)
		: SoftObjectPtr(MoveTemp(ObjectPath))
	{
	}

	/** Copy from an object already in memory */
	template <typename U>
	FORCEINLINE TSoftObjectPtr& operator=(const U* Object)
	{
		SoftObjectPtr = Object;
		return *this;
	}

	/** Assign from a nullptr */
	FORCEINLINE TSoftObjectPtr& operator=(TYPE_OF_NULLPTR)
	{
		SoftObjectPtr = nullptr;
		return *this;
	}

	/** Copy from a soft object path */
	FORCEINLINE TSoftObjectPtr& operator=(FSoftObjectPath ObjectPath)
	{
		SoftObjectPtr = MoveTemp(ObjectPath);
		return *this;
	}

	/** Copy from another soft pointer */
	template <class U, class = typename TEnableIf<TPointerIsConvertibleFromTo<U, T>::Value>::Type>
	FORCEINLINE TSoftObjectPtr& operator=(TSoftObjectPtr<U> Other)
	{
		SoftObjectPtr = MoveTemp(Other.SoftObjectPtr);
		return *this;
	}

	/**
	 * Compare soft pointers for equality
	 * Caution: Two soft pointers might not be equal to each other, but they both might return nullptr
	 *
	 * @param Other soft pointer to compare to
	 */
	FORCEINLINE friend bool operator==(const TSoftObjectPtr& Lhs, const TSoftObjectPtr& Rhs)
	{
		return Lhs.SoftObjectPtr == Rhs.SoftObjectPtr;
	}

	/**
	 * Compare soft pointers for equality
	 * Caution: Two soft pointers might not be equal to each other, but they both might return nullptr
	 *
	 * @param Other soft pointer to compare to
	 */
	FORCEINLINE friend bool operator==(const TSoftObjectPtr& Lhs, TYPE_OF_NULLPTR)
	{
		return Lhs.SoftObjectPtr == nullptr;
	}

	/**
	 * Compare soft pointers for equality
	 * Caution: Two soft pointers might not be equal to each other, but they both might return nullptr
	 *
	 * @param Other soft pointer to compare to
	 */
	FORCEINLINE friend bool operator==(TYPE_OF_NULLPTR, const TSoftObjectPtr& Rhs)
	{
		return nullptr == Rhs.SoftObjectPtr;
	}

	/**
	 * Compare soft pointers for inequality
	 * Caution: Two soft pointers might not be equal to each other, but they both might return nullptr
	 *
	 * @param Other soft pointer to compare to
	 */
	FORCEINLINE friend bool operator!=(const TSoftObjectPtr& Lhs, const TSoftObjectPtr& Rhs)
	{
		return Lhs.SoftObjectPtr != Rhs.SoftObjectPtr;
	}

	/**
	 * Compare soft pointers for inequality
	 * Caution: Two soft pointers might not be equal to each other, but they both might return nullptr
	 *
	 * @param Other soft pointer to compare to
	 */
	FORCEINLINE friend bool operator!=(const TSoftObjectPtr& Lhs, TYPE_OF_NULLPTR)
	{
		return Lhs.SoftObjectPtr != nullptr;
	}

	/**
	 * Compare soft pointers for inequality
	 * Caution: Two soft pointers might not be equal to each other, but they both might return nullptr
	 *
	 * @param Other soft pointer to compare to
	 */
	FORCEINLINE friend bool operator!=(TYPE_OF_NULLPTR, const TSoftObjectPtr& Rhs)
	{
		return nullptr != Rhs.SoftObjectPtr;
	}

	/**
	 * Dereference the soft pointer.
	 *
	 * @return nullptr if this object is gone or the lazy pointer was null, otherwise a valid UObject pointer
	 */
	FORCEINLINE T* Get() const
	{
		return reinterpret_cast<T*>(SoftObjectPtr.Get());
	}

	FORCEINLINE void Set(const FString& AssetPath)
	{
		SoftObjectPtr.ObjectID.SetPath(AssetPath);
	}

	/** Dereference the soft pointer */
	FORCEINLINE T& operator*() const
	{
		return *Get();
	}

	/** Dereference the soft pointer */
	FORCEINLINE T* operator->() const
	{
		return Get();
	}

	/**
	 * Test if this points to a live UObject
	 *
	 * @return true if Get() would return a valid non-null pointer
	 */
	FORCEINLINE bool IsValid() const
	{
		// This does the runtime type check
		return Get() != nullptr;
	}

	/** Returns the StringObjectPath that is wrapped by this SoftObjectPtr */
	FORCEINLINE const FSoftObjectPath& GetUniqueID() const
	{
		return SoftObjectPtr.ObjectID;
	}

	/** Returns the StringObjectPath that is wrapped by this SoftObjectPtr */
	FORCEINLINE const FSoftObjectPath& ToSoftObjectPath() const
	{
		return SoftObjectPtr.ObjectID;
	}

	/** Returns string representation of reference, in form /package/path.assetname */
	FORCEINLINE FString ToString() const
	{
		return ToSoftObjectPath().ToString();
	}

	/** Returns /package/path string, leaving off the asset name */
	FORCEINLINE FString GetLongPackageName() const
	{
		return ToSoftObjectPath().GetLongPackageName();
	}

	/** Returns assetname string, leaving off the /package/path part */
	FORCEINLINE FString GetAssetName() const
	{
		return ToSoftObjectPath().GetAssetName();
	}

	/** Dereference soft pointer to see if it points somewhere valid */
	FORCEINLINE explicit operator bool() const
	{
		return IsValid();
	}

	FSoftObjectPtr SoftObjectPtr;
};

template<class T> struct TIsPODType<TSoftObjectPtr<T> > { enum { Value = TIsPODType<FSoftObjectPtr>::Value }; };
template<class T> struct TIsWeakPointerType<TSoftObjectPtr<T> > { enum { Value = TIsWeakPointerType<FSoftObjectPtr>::Value }; };

/**
 * TSoftClassPtr is a templatized wrapper around FSoftObjectPtr that works like a TSubclassOf, it can be used in UProperties for blueprint subclasses
 */
template<class TClass = UObject>
class TSoftClassPtr
{
	template <class TClassA>
	friend class TSoftClassPtr;

public:
	/** Default constructor, will be null */
	FORCEINLINE TSoftClassPtr()
	{
	}

	/** Construct from another soft pointer */
	template <class TClassA, class = typename TEnableIf<TPointerIsConvertibleFromTo<TClassA, TClass>::Value>::Type>
	FORCEINLINE TSoftClassPtr(const TSoftClassPtr<TClassA>& Other)
		: SoftObjectPtr(Other.SoftObjectPtr)
	{
	}

	/** Construct from a class already in memory */
	FORCEINLINE TSoftClassPtr(const UClass* From)
		: SoftObjectPtr(From)
	{
	}

	/** Construct from a soft object path */
	explicit FORCEINLINE TSoftClassPtr(const FSoftObjectPath& ObjectPath)
		: SoftObjectPtr(ObjectPath)
	{
	}

	/** Copy from a class already in memory */
	FORCEINLINE void operator=(const UClass* From)
	{
		SoftObjectPtr = From;
	}

	/** Copy from a soft object path */
	FORCEINLINE void operator=(const FSoftObjectPath& ObjectPath)
	{
		SoftObjectPtr = ObjectPath;
	}

	/** Copy from another soft pointer */
	template<class TClassA, class = typename TEnableIf<TPointerIsConvertibleFromTo<TClassA, TClass>::Value>::Type>
	FORCEINLINE TSoftClassPtr& operator=(const TSoftObjectPtr<TClassA>& Other)
	{
		SoftObjectPtr = Other.SoftObjectPtr;
		return *this;
	}

	/**
	 * Compare soft pointers for equality
	 * Caution: Two soft pointers might not be equal to each other, but they both might return nullptr
	 *
	 * @param Other soft pointer to compare to
	 */
	FORCEINLINE friend bool operator==(const TSoftClassPtr& Lhs, const TSoftClassPtr& Rhs)
	{
		return Lhs.SoftObjectPtr == Rhs.SoftObjectPtr;
	}

	/**
	 * Compare soft pointers for inequality
	 * Caution: Two soft pointers might not be equal to each other, but they both might return nullptr
	 *
	 * @param Other soft pointer to compare to
	 */
	FORCEINLINE friend bool operator!=(const TSoftClassPtr& Lhs, const TSoftClassPtr& Rhs)
	{
		return Lhs.SoftObjectPtr != Rhs.SoftObjectPtr;
	}

	/**
	 * Dereference the soft pointer
	 *
	 * @return nullptr if this object is gone or the soft pointer was null, otherwise a valid UClass pointer
	 */
	FORCEINLINE UClass* Get() const
	{
		return dynamic_cast<UClass*>(SoftObjectPtr.Get());
	}

	FORCEINLINE void Set(const FString& AssetPath)
	{
		SoftObjectPtr.ObjectID.SetPath(AssetPath);
	}

	/** Dereference the soft pointer */
	FORCEINLINE UClass& operator*() const
	{
		return *Get();
	}

	/** Dereference the soft pointer */
	FORCEINLINE UClass* operator->() const
	{
		return Get();
	}

	/**
	 * Test if this points to a live UObject
	 *
	 * @return true if Get() would return a valid non-null pointer
	 */
	FORCEINLINE bool IsValid() const
	{
		// This also does the UClass type check
		return Get() != nullptr;
	}

	/** Returns the StringObjectPath that is wrapped by this SoftObjectPtr */
	FORCEINLINE const FSoftObjectPath& GetUniqueID() const
	{
		return SoftObjectPtr.ObjectID;
	}

	/** Returns the StringObjectPath that is wrapped by this SoftObjectPtr */
	FORCEINLINE const FSoftObjectPath& ToSoftObjectPath() const
	{
		return SoftObjectPtr.ObjectID;
	}

	/** Returns string representation of reference, in form /package/path.assetname  */
	FORCEINLINE FString ToString() const
	{
		return ToSoftObjectPath().ToString();
	}

	/** Returns /package/path string, leaving off the asset name */
	FORCEINLINE FString GetLongPackageName() const
	{
		return ToSoftObjectPath().GetLongPackageName();
	}

	/** Returns assetname string, leaving off the /package/path part */
	FORCEINLINE FString GetAssetName() const
	{
		return ToSoftObjectPath().GetAssetName();
	}

	/** Dereference soft pointer to see if it points somewhere valid */
	FORCEINLINE explicit operator bool() const
	{
		return IsValid();
	}

private:
	FSoftObjectPtr SoftObjectPtr;
};

template <class T> struct TIsPODType<TSoftClassPtr<T> > { enum { Value = TIsPODType<FSoftObjectPtr>::Value }; };
template <class T> struct TIsWeakPointerType<TSoftClassPtr<T> > { enum { Value = TIsWeakPointerType<FSoftObjectPtr>::Value }; };

[[deprecated("FAssetPtr was renamed to FSoftObjectPtr as it is not necessarily an asset")]]
typedef FSoftObjectPtr FAssetPtr;

// Not deprecating these yet as it will lead to too many warnings in games
//DEPRECATED(4.18, "TAssetPtr was renamed to TSoftObjectPtr as it is not necessarily an asset")
template<class T = UObject>
using TAssetPtr = TSoftObjectPtr<T>;

//DEPRECATED(4.18, "TAssetSubclassOf was renamed to TSoftClassPtr")
template<class TClass = UObject>
using TAssetSubclassOf = TSoftClassPtr<TClass>;