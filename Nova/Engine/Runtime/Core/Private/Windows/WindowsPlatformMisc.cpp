#include "Windows/WindowsPlatformMisc.h"

#include "KismetMemoryLibrary.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <combaseapi.h>

void FWindowsPlatformMisc::RequestExit(bool Force, const TCHAR* CallSite)
{
	FPlatformMisc::RequestExitWithStatus(Force, 0, CallSite);
}

void FWindowsPlatformMisc::RequestExitWithStatus(bool Force, uint8 ReturnCode, const TCHAR* CallSite)
{
	TerminateProcess(GetCurrentProcess(), ReturnCode);
}

void FWindowsPlatformMisc::ResumeMainThread()
{
    HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (Snapshot == INVALID_HANDLE_VALUE)
    {
        return;
    }

    THREADENTRY32 ThreadEntry;
    ThreadEntry.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(Snapshot, &ThreadEntry))
    {
        CloseHandle(Snapshot);
        return;
    }

    while (Thread32Next(Snapshot, &ThreadEntry))
    {
        if (ThreadEntry.th32OwnerProcessID != GetCurrentProcessId()) continue;

        auto Handle = OpenThread(THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION, false, ThreadEntry.th32ThreadID);

        if (Handle == INVALID_HANDLE_VALUE) continue;

        ResumeThread(Handle);
        CloseHandle(Handle);
    }

    CloseHandle(Snapshot);
}

bool FWindowsPlatformMisc::IsWithinImage(void* Target)
{
    auto Handle = GetModuleHandle(NULL);
    if (Handle == NULL)
    {
        return false;
    }

    MODULEINFO ModuleInfo;
    if (!GetModuleInformation(GetCurrentProcess(), Handle, &ModuleInfo, sizeof(ModuleInfo)))
    {
        return false;
    }

    uintptr_t BaseAddress = reinterpret_cast<uintptr_t>(ModuleInfo.lpBaseOfDll);
    uintptr_t EndAddress = BaseAddress + ModuleInfo.SizeOfImage;
    uintptr_t TargetAddress = reinterpret_cast<uintptr_t>(Target);

    return (TargetAddress >= BaseAddress && TargetAddress < EndAddress);
}

bool FWindowsPlatformMisc::IsDebuggerPresent()
{
    return ::IsDebuggerPresent();
}

void FWindowsPlatformMisc::PauseForDebugger()
{
    while (!FPlatformMisc::IsDebuggerPresent()) continue;
}