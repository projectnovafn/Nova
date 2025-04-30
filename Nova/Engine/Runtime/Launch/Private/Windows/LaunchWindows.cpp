#include "CoreMinimal.h"
#include "Async/Async.h"

#include "Windows/WindowsConsoleOutputDevice.h"

#include <Windows.h>

extern int32 GuardedMain(const TCHAR* CmdLine);

void Main()
{
    GLogConsole = new FWindowsConsoleOutputDevice();

    GuardedMain(GetCommandLineW());
}

bool DllMain(void*, int dwReason)
{
    if (dwReason == 1)
    {
        AsyncThread(Main);
    }

    return true;
}