#pragma once

#include "CoreTypes.h"
#include "CoreFwd.h"
#include "ObjectMacros.h"
#include "UObject/UnrealNames.h"

#define ANY_PACKAGE (UObject*)-1

//
// Flags for loading objects.
//
enum ELoadFlags
{
    LOAD_None = 0x00000000,	// No flags.
    LOAD_Async = 0x00000001,	// Loads the package using async loading path/ reader
    LOAD_NoWarn = 0x00000002,	// Don't display warning if load fails.
    LOAD_EditorOnly = 0x00000004, // Load for editor-only purposes and by editor-only code
    LOAD_ResolvingDeferredExports = 0x00000008,	// Denotes that we should not defer export loading (as we're resolving them)
    LOAD_Verify = 0x00000010,	// Only verify existance; don't actually load.
    LOAD_AllowDll = 0x00000020,	// Allow plain DLLs.
    //	LOAD_Unused						= 0x00000040
    LOAD_NoVerify = 0x00000080,   // Don't verify imports yet.
    LOAD_IsVerifying = 0x00000100,		// Is verifying imports
    //	LOAD_Unused						= 0x00000200,
    //	LOAD_Unused						= 0x00000400,
    //	LOAD_Unused						= 0x00000800,
    LOAD_DisableDependencyPreloading = 0x00001000,	// Bypass dependency preloading system
    LOAD_Quiet = 0x00002000,   // No log warnings.
    LOAD_FindIfFail = 0x00004000,	// Tries FindObject if a linker cannot be obtained (e.g. package is currently being compiled)
    LOAD_MemoryReader = 0x00008000,	// Loads the file into memory and serializes from there.
    LOAD_NoRedirects = 0x00010000,	// Never follow redirects when loading objects; redirected loads will fail
    LOAD_ForDiff = 0x00020000,	// Loading for diffing.
    LOAD_PackageForPIE = 0x00080000,   // This package is being loaded for PIE, it must be flagged as such immediately
    LOAD_DeferDependencyLoads = 0x00100000,   // Do not load external (blueprint) dependencies (instead, track them for deferred loading)
    LOAD_ForFileDiff = 0x00200000,	// Load the package (not for diffing in the editor), instead verify at the two packages serialized output are the same, if they are not then debug break so that you can get the callstack and object information
    LOAD_DisableCompileOnLoad = 0x00400000,	// Prevent this load call from running compile on load for the loaded blueprint (intentionally not recursive, dependencies will still compile on load)
};

enum class EDuplicateMode
{
    /** No specific information about the reason for duplication */
    Normal,
    /** Object is being duplicated as part of a world duplication */
    World,
    /** Object is being duplicated as part of the process for entering Play In Editor */
    PIE
};

class UObject* StaticFindObject(class UClass* ObjectClass, UObject* InObjectPackage, const TCHAR* ObjectName, bool ExactClass);

FORCEINLINE UObject* StaticFindObject(const TCHAR* ObjectName)
{
    return StaticFindObject(nullptr, nullptr, ObjectName, false);
}

template< class T >
FORCEINLINE T* FindObject(UObject* Outer, const WIDECHAR* Name, bool ExactClass = false)
{
    return reinterpret_cast<T*>(StaticFindObject(T::StaticClass(), Outer, Name, ExactClass));
}

template< class T >
FORCEINLINE T* FindObject(const WIDECHAR* Name, bool ExactClass = false)
{
    return reinterpret_cast<T*>(StaticFindObject(T::StaticClass(), nullptr, Name, ExactClass));
}

UObject* StaticLoadObject(class UClass* ObjectClass, UObject* InOuter, const TCHAR* InName, const TCHAR* Filename = nullptr, uint32 LoadFlags = 0, UPackageMap* Sandbox = nullptr, bool bAllowObjectReconciliation = true);

FORCEINLINE UObject* StaticLoadObject(class UClass* ObjectClass, const wchar_t* OrigInName)
{
    return StaticLoadObject(ObjectClass, nullptr, OrigInName);
}

template< class T >
FORCEINLINE T* LoadObject(UObject* Outer, const wchar_t* Name, const wchar_t* Filename = nullptr, uint32 LoadFlags = 0, UPackageMap* Sandbox = nullptr)
{
    return (T*)StaticLoadObject(T::StaticClass(), Outer, Name, Filename, LoadFlags, Sandbox);
}
template< class T >
FORCEINLINE T* LoadObject(const wchar_t* Name, const wchar_t* Filename = nullptr, uint32 LoadFlags = 0, UPackageMap* Sandbox = nullptr)
{
    return (T*)StaticLoadObject(T::StaticClass(), nullptr, Name, Filename, LoadFlags, Sandbox);
}

UObject* StaticDuplicateObject(UObject const* SourceObject, UObject* DestOuter, const FName DestName = NAME_None, EObjectFlags FlagMask = RF_AllFlags, 
    UClass* DestClass = nullptr, EDuplicateMode DuplicateMode = EDuplicateMode::Normal, EInternalObjectFlags InternalFlagsMask = EInternalObjectFlags::AllFlags);

template< class T >
T* DuplicateObject(T const* SourceObject, UObject* Outer, const FName Name = NAME_None)
{
    return static_cast<T*>(StaticDuplicateObject(SourceObject, Outer, Name, RF_AllFlags, T::StaticClass()));
}

UObject* GetTransientPackage();