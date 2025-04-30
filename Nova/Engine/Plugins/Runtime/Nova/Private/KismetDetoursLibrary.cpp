#include "KismetDetoursLibrary.h"
#include "CoreUObject.h"

#include <Windows.h>
#include <detours.h>

#include "vendor/memcury.h"

#define CREATE_HOOK(Detour, Original) DetourTransactionBegin(); DetourUpdateThread(GetCurrentThread()); DetourAttach(&(void*&)Original, Detour); DetourTransactionCommit();

uint8_t* AllocatePageNearAddress(void* targetAddr)
{
    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);
    const uint64_t PageSize = SysInfo.dwPageSize;

    uint64_t StartAddr = (uint64_t(targetAddr) & ~(PageSize - 1));
    uint64_t MinAddr = min(StartAddr - 0x7FFFFF00, (uint64_t)SysInfo.lpMinimumApplicationAddress);
    uint64_t MaxAddr = max(StartAddr + 0x7FFFFF00, (uint64_t)SysInfo.lpMaximumApplicationAddress);

    uint64_t StartPage = (StartAddr - (StartAddr % PageSize));

    for (uint64_t PageOffset = 1; PageOffset; PageOffset++)
    {
        uint64_t ByteOffset = PageOffset * PageSize;
        uint64_t HighAddr = StartPage + ByteOffset;
        uint64_t LowAddr = (StartPage > ByteOffset) ? StartPage - ByteOffset : 0;

        bool NeedsExit = HighAddr > MaxAddr && LowAddr < MinAddr;

        if (HighAddr < MaxAddr)
        {
            void* OutAddr = VirtualAlloc((void*)HighAddr, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (OutAddr)
                return (uint8_t*)OutAddr;
        }

        if (LowAddr > MinAddr)
        {
            void* OutAddr = VirtualAlloc((void*)LowAddr, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (OutAddr != nullptr)
                return (uint8_t*)OutAddr;
        }

        if (NeedsExit)
        {
            break;
        }
    }

    return nullptr;
}

static bool IsFunctionEnd(uint8_t* Address)
{
    int Align = 0x10 - (uintptr_t(Address) % 0x10);

    return Address[0] == 0xC3 &&
        Address[Align] == 0x40 &&
        ((Address[-1] >= 0x58 && Address[-1] <= 0x5F) ||
            (Address[-2] == 0x41 && Address[-1] >= 0x58 && Address[-1] <= 0x5F));
}

static uint8_t* FindFunctionEnd(uint8_t* Address)
{
    for (int i = 0; i < 0x1000; i++)
    {
        if (IsFunctionEnd(&Address[i]))
            return &Address[i + 1];
        if ((Address[i] % 0x10 == 0) && (Address[i] == 0x40 && (Address[i + 1] >= 0x50 && Address[i + 1] <= 0x57) && (Address[i + 2] >= 0x50 && Address[i + 2] <= 0x57)))
        {
            return &Address[i + 1];
        }
    }

    return NULL;
}

static uint32_t GetExecIdx(UFunction* Function, int Skip = 0)
{
    auto Address = (uint8*)Function->Func;

    for (int i = 0; i < 0x1000; i++)
    {
        if ((Address[i] == 0xFF) && (Address[i + 1] == 0xA0 || Address[i + 1] == 0x93 || Address[i + 1] == 0x90 || Address[i + 1] == 0x92))
        {
            if (Skip != 0)
            {
                Skip--;
                continue;
            }

            auto VTableIdx = (*(uint32_t*)&Address[i + 2]) / sizeof(uint32_t*);

            if (VTableIdx > 2500)
                return -1;

            return VTableIdx;
        }
    }

    return -1;
}

static uint8_t* GetExecImplRef(UFunction* Function, const std::vector<uint8_t>& ScanBytes, int Skip = 0)
{
    auto Address = (uint8*)Function->Func;

    if (ScanBytes.empty())
    {
        auto FunctionEnd = FindFunctionEnd(Address + 1);
        auto Scanner = Memcury::Scanner(FunctionEnd);

        if (!Scanner.IsValid())
            return NULL;

        return Scanner.ScanFor({ 0xE8 }, false, Skip).GetAs<uint8_t*>();
    }
    else
    {
        auto Scanner = Memcury::Scanner(Address);

        if (!Scanner.IsValid())
            return NULL;

        return Scanner.ScanFor(ScanBytes)
            .ScanFor({ 0xE8 })
            .GetAs<uint8_t*>();
    }
}

static uint8_t* GetExecImpl(UFunction* Function, const std::vector<uint8_t>& ScanBytes)
{
    return Memcury::Scanner(GetExecImplRef(Function, ScanBytes)).RelativeOffset(1).GetAs<uint8_t*>();
}

static uint8_t* GetCallImpl(UFunction* Function, int Skip, const std::vector<uint8_t>& ScanBytes)
{
    auto FuncName = Function->GetName();
    auto FuncPtrAddr = Memcury::Scanner::FindStringRef(*FuncName, Skip)
        .ScanFor({ 0x48 })
        .RelativeOffset(3)
        .GetAs<uint8_t*>();

    uint8_t* FuncAddr = NULL;

    if (ScanBytes.empty())
    {
        FuncAddr = Memcury::Scanner::FindPointerRef(FuncPtrAddr, 1)
            .FindFunctionBoundary()
            .GetAs<uint8_t*>();
    }
    else
    {
        FuncAddr = Memcury::Scanner::FindPointerRef(FuncPtrAddr, 1)
            .ScanFor(ScanBytes, false)
            .GetAs<uint8_t*>();
    }

    return FuncAddr;
}

void UKismetDetoursLibrary::AddHookInternal(void** Target, void* Detour, EDetourType Type)
{
	if (Type == EDetourType::Detour)
	{
		DetourAttach(Target, Detour);
	}
	else if (Type == EDetourType::Rel32Call)
	{
        auto Impl = (uint8*)(*Target);
        auto NearPage = AllocatePageNearAddress(Impl);

        if (!NearPage)
        {
            return;
        }

        uint8_t Shellcode[] =
        {
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp [$+6]
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        if (Detour != NULL)
        {
            memcpy(Shellcode + 6, &Detour, 8);
            memcpy(NearPage, Shellcode, sizeof(Shellcode));

            auto Offset = NearPage - (Impl + 5);

            memcpy(Impl + 1, &Offset, sizeof(int));
        }
        else
        {
            memset(Impl, 0x90, sizeof(int) + 1);
        }
	}
    else if (Type == EDetourType::VFSwap)
    {
        while (true)
        {
            auto FuncRef = Memcury::Scanner::FindDataPointerRef(*Target);

            if (!FuncRef.IsValid()) break;

            *FuncRef.GetAs<void**>() = Detour;
        }
    }
}

void UKismetDetoursLibrary::RemoveHookInternal(void** Target, void* Detour, EDetourType Type)
{
	if (Type == EDetourType::Detour)
	{
		DetourDetach(Target, Detour);
	}
}

void UKismetDetoursLibrary::BeginTransaction()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
}

void UKismetDetoursLibrary::CommitTransaction()
{
	DetourTransactionCommit();
}

bool UKismetDetoursLibrary::AddReflectedDetourInternal(UObject* Object, UFunction* Function, EReflectedDetourType Type, void* Detour, void** Original, int Skip, const std::vector<uint8_t>& ScanBytes)
{
    if (!Function)
    {
        UE_LOG(LogDetours, Error, TEXT("AddHook: Invalid Function!"));

        return false;
    }

    if (Type == EReflectedDetourType::VFSwap)
    {
        auto VFI = GetExecIdx(Function, Skip);

        if (VFI == -1)
        {
            UE_LOG(LogDetours, Error, TEXT("VFSwap: %s, Failed to find VFI!"), *Function->GetName());

            return false;
        }

        if (!Object)
        {
            UE_LOG(LogDetours, Error, TEXT("VFSwap: %s, Invalid Object!"), *Function->GetName());

            return false;
        }

        if (Detour != NULL)
        {
            if (Original)
                *Original = Object->VTable[VFI];

            Object->VTable[VFI] = (void*)(Detour);
        }
        else
        {
            *(uint8*)Object->VTable[VFI] = 0xC3;
        }

        UE_LOG(LogDetours, Log, TEXT("AddHook: %s %i Hook Enabled!"), *Function->GetName(), VFI);
    }
    else if (Type == EReflectedDetourType::ExecSwap)
    {
        if (Detour != NULL)
        {
            if (Original)
                *Original = Function->Func;

            Function->Func = (uint8*)(Detour);
        }
        else
        {
            *(uint8*)Function->Func = 0xC3;
        }

        UE_LOG(LogDetours, Log, TEXT("AddHook: %s %p Hook Enabled!"), *Function->GetName(), Function->Func);
    }
    else if (Type == EReflectedDetourType::ImplSwap)
    {
        auto Impl = GetExecImplRef(Function, ScanBytes, Skip);

        if (!Impl)
        {
            UE_LOG(LogDetours, Error, TEXT("ImplSwap: %s, Failed to find Impl"), *Function->GetName());

            return false;
        }

        auto NearPage = AllocatePageNearAddress(Impl);

        if (!NearPage)
        {
            UE_LOG(LogDetours, Error, TEXT("AddHook: %s %p Failed to Allocate NearPage!"), *Function->GetName(), Function->Func);

            return false;
        }

        uint8_t Shellcode[] =
        {
            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp [$+6]
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        if (Detour != NULL)
        {
            memcpy(Shellcode + 6, &Detour, 8);
            memcpy(NearPage, Shellcode, sizeof(Shellcode));

            auto Offset = NearPage - (Impl + 5);

            memcpy(Impl + 1, &Offset, sizeof(int));
        }
        else
        {
            memset(Impl, 0x90, sizeof(int) + 1);
        }

        UE_LOG(LogDetours, Log, TEXT("AddHook: %s %p Hook Enabled!"), *Function->GetName(), Impl);
    }
    else if (Type == EReflectedDetourType::ImplDetour)
    {
        auto Impl = GetExecImpl(Function, ScanBytes);

        if (!Impl)
        {
            UE_LOG(LogDetours, Error, TEXT("ImplDetour: %s, Failed to find Impl"), *Function->GetName());

            return false;
        }

        if (Detour != NULL)
        {
            auto TempOriginal = (void (*)())(Impl);
            CREATE_HOOK(Detour, TempOriginal);

            if (Original)
                *Original = TempOriginal;
        }
        else
        {
            *(uint8*)Impl = 0xC3;
        }

        UE_LOG(LogDetours, Log, TEXT("AddHook: %s %p Hook Enabled!"), *Function->GetName(), Impl);
    }
    else if (Type == EReflectedDetourType::CallDetour)
    {
        auto Impl = GetCallImpl(Function, Skip, ScanBytes);

        if (!Impl)
        {
            UE_LOG(LogDetours, Error, TEXT("CallDetour: %s, Failed to find Impl"), *Function->GetName());

            return false;
        }

        if (Detour != NULL)
        {
            auto TempOriginal = (void (*)())(Impl);
            CREATE_HOOK(Detour, TempOriginal);

            if (Original)
                *Original = TempOriginal;
        }
        else
        {
            *(uint8*)Impl = 0xC3;
        }

        UE_LOG(LogDetours, Log, TEXT("AddHook: %s %p Hook Enabled!"), *Function->GetName(), Impl);
    }

    return true;
}