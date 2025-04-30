#pragma once

#include "CoreTypes.h"
#include "Containers/UnrealString.h"

struct FOutputDeviceHelper
{
	static const TCHAR* VerbosityToString(ELogVerbosity::Type Verbosity);
	static FString FormatLogLine(ELogVerbosity::Type Verbosity, const TCHAR* Category, const TCHAR* Message = nullptr, ELogTimes::Type LogTime = ELogTimes::None, const double Time = -1.0);
};