#include "UObject/SoftObjectPath.h"

FThreadSafeCounter FSoftObjectPath::CurrentTag(1);

UObject* FSoftObjectPath::ResolveObject() const
{
    // Don't try to resolve if we're saving a package because StaticFindObject can't be used here
    // and we usually don't want to force references to weak pointers while saving.
    if (IsNull())
    {
        return nullptr;
    }

    FString PathString = ToString();

    return LoadObject<UObject>(nullptr, *PathString, nullptr, 0, nullptr);
}

FString FSoftObjectPath::ToString() const
{
    // Most of the time there is no sub path so we can do a single string allocation
    FString AssetPathString = GetAssetPathString();
    if (SubPathString.IsEmpty())
    {
        return AssetPathString;
    }
    FString FullPathString;

    // Preallocate to correct size and then append strings
    FullPathString.Empty(AssetPathString.Len() + SubPathString.Len() + 1);
    FullPathString.Append(AssetPathString);
    FullPathString.AppendChar(':');
    FullPathString.Append(SubPathString);
    return FullPathString;
}