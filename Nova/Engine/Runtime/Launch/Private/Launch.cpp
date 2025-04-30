#include "Core.h"
#include "Nova.h"

int32 GuardedMain(const TCHAR* CmdLine)
{
	GNova = new UNova();
	GNova->Init();

	return 0;
}