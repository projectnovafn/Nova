#include "HAL/UnrealMemory.h"
#include "KismetMemoryLibrary.h"

void* FMemory::Realloc(void* Original, SIZE_T Count, uint32 Alignment)
{
    static auto Realloc = UKismetMemoryLibrary::Get<void* (*)(void*, SIZE_T, uint32)>(L"FMemory::Realloc");

    return Realloc(Original, Count, Alignment);
}

void FMemory::Free(void* Original)
{
    FMemory::Realloc(Original, 0, 0);
}

SIZE_T FMemory::QuantizeSize(SIZE_T Count, uint32 Alignment)
{
    return Count;
}