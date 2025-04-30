#pragma once
#include "CoreMinimal.h"
#include <functional>
#include <map>

#define DEFINE_MEMORY_VARIABLE(Name, FunctionName) \
    void* Name##_Function(); \
    static auto _##Name##_Initializer = []() { \
        UKismetMemoryLibrary::Add(TEXT(FunctionName), Name##_Function); \
        return 0; \
    }(); \
    void* Name##_Function()

#define DEFINE_MEMORY_OFFSET(Name, FunctionName) \
    uint32 Name##_Function(); \
    static auto _##Name##_Initializer = []() { \
        UKismetMemoryLibrary::Add(TEXT(FunctionName), Name##_Function); \
        return 0; \
    }(); \
    uint32 Name##_Function()

class UKismetMemoryLibrary
{
    using FunctionType = void*(*)();
    using OffsetFunctionType = uint32(*)();

    static void* GetInternal(const TCHAR* Name);
    static void AddInternal(const TCHAR* Name, FunctionType Function);

    static uint32 GetOffsetInternal(const TCHAR* Name);
    static void AddOffsetInternal(const TCHAR* Name, OffsetFunctionType Function);

public:

    static void LoadCache();

    static void Add(const TCHAR* Name, FunctionType Function)
    {
        UKismetMemoryLibrary::AddInternal(Name, Function);
    }

    static void Add(const TCHAR* Name, OffsetFunctionType Function)
    {
        UKismetMemoryLibrary::AddOffsetInternal(Name, Function);
    }
    
	template <typename T>
	static T Get(const TCHAR* Name)
	{
        return (T)UKismetMemoryLibrary::GetInternal(Name);
    }

    static uint32 Get(const TCHAR* Name)
    {
        return UKismetMemoryLibrary::GetOffsetInternal(Name);
    }
};