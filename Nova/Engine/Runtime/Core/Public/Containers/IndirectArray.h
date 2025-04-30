#pragma once
#include "CoreMinimal.h"

template<typename T>
class TIndirectArray
{
public:
    __forceinline T** GetData() { return Array.GetData(); }
    __forceinline int32 Num() { return Array.Num(); }
    __forceinline int32 Max() { return Array.Max(); }

    __forceinline T& operator[](int32 Index)
    {
        return *reinterpret_cast<T*>(Array[Index]);
    }
private:
    TArray<void*> Array;
};