#pragma once
#include "Windows\WindowsRunnableThread.h"

uint32 FRunnableThreadWin::GuardedRun()
{
	return this->Run();
}

uint32 FRunnableThreadWin::Run()
{
	uint32 ExitCode = 1;

	if (Runnable->Init() == true)
	{
		ExitCode = Runnable->Run();

		Runnable->Exit();
	}

	return ExitCode;
}