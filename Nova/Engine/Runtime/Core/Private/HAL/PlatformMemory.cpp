#include "HAL/PlatformMemory.h"
#include <Misc/AssertionMacros.h>

void FPlatformMemory::MemswapGreaterThan8(void* RESTRICT Ptr1, void* RESTRICT Ptr2, SIZE_T Size)
{
    union PtrUnion
    {
        void* PtrVoid;
        uint8* Ptr8;
        uint16* Ptr16;
        uint32* Ptr32;
        uint64* Ptr64;
        UPTRINT PtrUint;
    };

    PtrUnion Union1 = { Ptr1 };
    PtrUnion Union2 = { Ptr2 };

    check(Union1.PtrVoid && Union2.PtrVoid);
    check(Size > 8);

    if (Union1.PtrUint & 1)
    {
        Valswap(*Union1.Ptr8++, *Union2.Ptr8++);
        Size -= 1;
    }
    if (Union1.PtrUint & 2)
    {
        Valswap(*Union1.Ptr16++, *Union2.Ptr16++);
        Size -= 2;
    }
    if (Union1.PtrUint & 4)
    {
        Valswap(*Union1.Ptr32++, *Union2.Ptr32++);
        Size -= 4;
    }

    uint32 CommonAlignment = FMath::Min(FMath::CountTrailingZeros((uint32)(Union1.PtrUint - Union2.PtrUint)), 3u);
    switch (CommonAlignment)
    {
    default:
        for (; Size >= 8; Size -= 8)
        {
            Valswap(*Union1.Ptr64++, *Union2.Ptr64++);
        }

    case 2:
        for (; Size >= 4; Size -= 4)
        {
            Valswap(*Union1.Ptr32++, *Union2.Ptr32++);
        }

    case 1:
        for (; Size >= 2; Size -= 2)
        {
            Valswap(*Union1.Ptr16++, *Union2.Ptr16++);
        }

    case 0:
        for (; Size >= 1; Size -= 1)
        {
            Valswap(*Union1.Ptr8++, *Union2.Ptr8++);
        }
    }
}