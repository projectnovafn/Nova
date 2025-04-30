#pragma once

#include "CoreTypes.h"
#include "Templates/IsPointer.h"
#include "HAL/PlatformMemory.h"

enum
{
    // Default allocator alignment. If the default is specified, the allocator applies to engine rules.
    // Blocks >= 16 bytes will be 16-byte-aligned, Blocks < 16 will be 8-byte aligned. If the allocator does
    // not support allocation alignment, the alignment will be ignored.
    DEFAULT_ALIGNMENT = 0,

    // Minimum allocator alignment
    MIN_ALIGNMENT = 8,
};

struct FMemory
{
    FORCEINLINE static void* Malloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT) { return Realloc(nullptr, Count, Alignment); }
    static void* Realloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
    static void Free(void* Original);
    static SIZE_T QuantizeSize(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);

    static FORCEINLINE void* Memmove(void* Dest, const void* Src, SIZE_T Count)
    {
        return FPlatformMemory::Memmove(Dest, Src, Count);
    }

    static FORCEINLINE int32 Memcmp(const void* Buf1, const void* Buf2, SIZE_T Count)
    {
        return FPlatformMemory::Memcmp(Buf1, Buf2, Count);
    }

    static FORCEINLINE void* Memset(void* Dest, uint8 Char, SIZE_T Count)
    {
        return FPlatformMemory::Memset(Dest, Char, Count);
    }

    template< class T >
    static FORCEINLINE void Memset(T& Src, uint8 ValueToSet)
    {
        static_assert(!TIsPointer<T>::Value, "For pointers use the three parameters function");
        Memset(&Src, ValueToSet, sizeof(T));
    }

    static FORCEINLINE void* Memzero(void* Dest, SIZE_T Count)
    {
        return FPlatformMemory::Memzero(Dest, Count);
    }

    template< class T >
    static FORCEINLINE void Memzero(T& Src)
    {
        static_assert(!TIsPointer<T>::Value, "For pointers use the two parameters function");
        Memzero(&Src, sizeof(T));
    }

    static FORCEINLINE void* Memcpy(void* Dest, const void* Src, SIZE_T Count)
    {
        return FPlatformMemory::Memcpy(Dest, Src, Count);
    }

    template< class T >
    static FORCEINLINE void Memcpy(T& Dest, const T& Src)
    {
        static_assert(!TIsPointer<T>::Value, "For pointers use the three parameters function");
        Memcpy(&Dest, &Src, sizeof(T));
    }

    static FORCEINLINE void* BigBlockMemcpy(void* Dest, const void* Src, SIZE_T Count)
    {
        return FPlatformMemory::BigBlockMemcpy(Dest, Src, Count);
    }

    static FORCEINLINE void* StreamingMemcpy(void* Dest, const void* Src, SIZE_T Count)
    {
        return FPlatformMemory::StreamingMemcpy(Dest, Src, Count);
    }

    static FORCEINLINE void Memswap(void* Ptr1, void* Ptr2, SIZE_T Size)
    {
        FPlatformMemory::Memswap(Ptr1, Ptr2, Size);
    }
};