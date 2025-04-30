#include "UObject/UObjectGlobals.h"
#include "UObject/Class.h"

#include "KismetMemoryLibrary.h"

UObject* StaticFindObject(UClass* ObjectClass, UObject* InObjectPackage, const TCHAR* ObjectName, bool ExactClass)
{
    static auto StaticFindObject = UKismetMemoryLibrary::Get<UObject * (__fastcall*)(UClass*, UObject*, const TCHAR*, bool)>(L"StaticFindObject");

    return StaticFindObject(ObjectClass, InObjectPackage, ObjectName, ExactClass);
}

UObject* StaticLoadObject(UClass* ObjectClass, UObject* InOuter, const TCHAR* InName, const TCHAR* Filename,
    uint32 LoadFlags, UPackageMap* Sandbox, bool bAllowObjectReconciliation)
{
    static auto StaticLoadObject = UKismetMemoryLibrary::Get<UObject * (__fastcall*)(UClass*, UObject*, const TCHAR*, const TCHAR*, uint32, UPackageMap*, bool)>(L"StaticLoadObject");

    return StaticLoadObject(ObjectClass, InOuter, InName, Filename, LoadFlags, Sandbox, bAllowObjectReconciliation);
}

UObject* StaticDuplicateObject(UObject const* SourceObject, UObject* DestOuter, const FName DestName, EObjectFlags FlagMask, 
    UClass* DestClass, EDuplicateMode DuplicateMode, EInternalObjectFlags InternalFlagsMask)
{
    static auto StaticDuplicateObject = UKismetMemoryLibrary::Get<UObject * (__fastcall*)(UObject const*, UObject*, const FName, EObjectFlags, UClass*, EDuplicateMode, EInternalObjectFlags)>(L"StaticDuplicateObject");

    return StaticDuplicateObject(SourceObject, DestOuter, DestName, FlagMask, DestClass, DuplicateMode, InternalFlagsMask);
}

UObject* GetTransientPackage()
{
    static auto Transient = StaticFindObject(L"/Engine/Transient");

    return Transient;
}