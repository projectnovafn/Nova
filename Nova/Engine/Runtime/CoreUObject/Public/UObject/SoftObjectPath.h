#pragma once

#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"
#include "Templates/Casts.h"
#include "HAL/ThreadSingleton.h"
#include "HAL/ThreadSafeCounter.h"
#include "UObjectGlobals.h"

#include "Class.h"

/**
 * A struct that contains a string reference to an object, either a top level asset or a subobject.
 * This can be used to make soft references to assets that are loaded on demand.
 * This is stored internally as an FName pointing to the top level asset (/package/path.assetname) and an option a string subobject path.
 * If the MetaClass metadata is applied to a UProperty with this the UI will restrict to that type of asset.
 */
struct FSoftObjectPath
{
    FSoftObjectPath() {}

    /** Construct from an asset FName and subobject pair */
    FSoftObjectPath(FName InAssetPathName, FString InSubPathString)
        : AssetPathName(InAssetPathName)
        , SubPathString(MoveTemp(InSubPathString))
    {}

    /** Returns string representation of reference, in form /package/path.assetname[:subpath] */
    FString ToString() const;

    /** Returns the entire asset path as an FName, including both package and asset but not sub object */
    FORCEINLINE FName GetAssetPathName() const
    {
        return AssetPathName;
    }

    /** Returns string version of asset path, including both package and asset but not sub object */
    FORCEINLINE FString GetAssetPathString() const
    {
        if (AssetPathName == NAME_None)
        {
            return FString();
        }

        return AssetPathName.ToString();
    }

    /** Returns the sub path, which is often empty */
    FORCEINLINE const FString& GetSubPathString() const
    {
        return SubPathString;
    }

    /** Returns /package/path, leaving off the asset name and sub object */
    FString GetLongPackageName() const
    {
        FString PackageName;
        GetAssetPathString().Split(L".", &PackageName, nullptr, ESearchCase::CaseSensitive, ESearchDir::FromStart);
        return PackageName;
    }

    /** Returns assetname string, leaving off the /package/path part and sub object */
    FString GetAssetName() const
    {
        FString AssetName;
        GetAssetPathString().Split(L".", nullptr, &AssetName, ESearchCase::CaseSensitive, ESearchDir::FromStart);
        return AssetName;
    }

    /**
     * Attempts to find a currently loaded object that matches this path
     *
     * @return Found UObject, or nullptr if not currently in memory
     */
    UObject* ResolveObject() const;

    /** Resets reference to point to null */
    void Reset()
    {
        AssetPathName = NAME_None;
        SubPathString.Reset();
    }

    /** Check if this could possibly refer to a real object, or was initialized to null */
    FORCEINLINE bool IsValid() const
    {
        return AssetPathName != NAME_None;
    }

    /** Checks to see if this is initialized to null */
    FORCEINLINE bool IsNull() const
    {
        return AssetPathName == NAME_None;
    }

    /** Check if this represents an asset, meaning it is not null but does not have a sub path */
    FORCEINLINE bool IsAsset() const
    {
        return AssetPathName != NAME_None && SubPathString.IsEmpty();
    }

    /** Check if this represents a sub object, meaning it has a sub path */
    FORCEINLINE bool IsSubobject() const
    {
        return AssetPathName != NAME_None && !SubPathString.IsEmpty();
    }

    /** Code needed by FSoftObjectPtr internals */
    static int32 GetCurrentTag()
    {
        return CurrentTag.GetValue();
    }
    static int32 InvalidateTag()
    {
        return CurrentTag.Increment();
    }

    void SetPath(const TCHAR* Path)
    {
        AssetPathName = Path;
    }

private:
    /** Asset path, patch to a top level object in a package. This is /package/path.assetname */
    FName AssetPathName;

    /** Optional FString for subobject within an asset. This is the sub path after the : */
    FString SubPathString;

    /** Global counter that determines when we need to re-search based on path because more objects have been loaded **/
    static FThreadSafeCounter CurrentTag;

    /** Package names currently being duplicated, needed by FixupForPIE */
    static TSet<FName> PIEPackageNames;

    friend struct Z_Construct_UScriptStruct_FSoftObjectPath_Statics;
};

/**
 * A struct that contains a string reference to a class, can be used to make soft references to classes
 */
struct FSoftClassPath : public FSoftObjectPath
{
    FSoftClassPath()
    { }

    FSoftClassPath(FSoftClassPath const& Other)
        : FSoftObjectPath(Other)
    { }

    /**
    * Attempts to load the class.
    * @return Loaded UObject, or null if the class fails to load, or if the reference is not valid.
    */
    template<typename T>
    UClass* TryLoadClass() const
    {
        if (IsValid())
        {
            return LoadClass<T>(nullptr, *ToString(), nullptr, LOAD_None, nullptr);
        }

        return nullptr;
    }

private:
    /** Forbid creation for UObject. This class is for UClass only. Use FSoftObjectPath instead. */
    FSoftClassPath(const UObject* InObject) { }
};

// Not deprecating these yet as it will lead to too many warnings in games
//DEPRECATED(4.18, "FStringAssetReference was renamed to FSoftObjectPath as it is now not always a string and can also refer to a subobject")
typedef FSoftObjectPath FStringAssetReference;

//DEPRECATED(4.18, "FStringClassReference was renamed to FSoftClassPath")
typedef FSoftClassPath FStringClassReference;

/** Options for how to set soft object path collection */
enum class ESoftObjectPathCollectType : uint8
{
    /** References is not tracked in any situation, transient reference */
    NeverCollect,
    /** Editor only reference, this is tracked for redirector fixup but not for cooking */
    EditorOnlyCollect,
    /** Game reference, this is gathered for both redirector fixup and cooking */
    AlwaysCollect,
};

/** Rules for actually serializing the internals of soft object paths */
enum class ESoftObjectPathSerializeType : uint8
{
    /** Never serialize the raw names */
    NeverSerialize,
    /** Only serialize if the archive has no size */
    SkipSerializeIfArchiveHasSize,
    /** Always serialize the soft object path internals */
    AlwaysSerialize,
};

class FSoftObjectPathThreadContext : public TThreadSingleton<FSoftObjectPathThreadContext>
{
    friend TThreadSingleton<FSoftObjectPathThreadContext>;
    friend struct FSoftObjectPathSerializationScope;

    FSoftObjectPathThreadContext() {}

    struct FSerializationOptions
    {
        FName PackageName;
        FName PropertyName;
        ESoftObjectPathCollectType CollectType;
        ESoftObjectPathSerializeType SerializeType;

        FSerializationOptions() : CollectType(ESoftObjectPathCollectType::AlwaysCollect) {}
        FSerializationOptions(FName InPackageName, FName InPropertyName, ESoftObjectPathCollectType InCollectType, ESoftObjectPathSerializeType InSerializeType) : PackageName(InPackageName), PropertyName(InPropertyName), CollectType(InCollectType), SerializeType(InSerializeType) {}
    };

    TArray<FSerializationOptions> OptionStack;
};