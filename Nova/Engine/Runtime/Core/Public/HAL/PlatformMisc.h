#pragma once

#include "CoreTypes.h"
#include "GenericPlatform/GenericPlatformMisc.h"

#if PLATFORM_WINDOWS
#include "Windows\WindowsPlatformMisc.h"
#endif

#ifndef PLATFORM_USES_ANSI_STRING_FOR_EXTERNAL_PROFILING
#error PLATFORM_USES_ANSI_STRING_FOR_EXTERNAL_PROFILING is not defined.
#endif

#ifndef PLATFORM_LIMIT_PROFILER_UNIQUE_NAMED_EVENTS
#if defined(FRAMEPRO_ENABLED) && FRAMEPRO_ENABLED

#define PLATFORM_LIMIT_PROFILER_UNIQUE_NAMED_EVENTS 1
#else
#define PLATFORM_LIMIT_PROFILER_UNIQUE_NAMED_EVENTS 0
#endif
#endif

#ifndef PLATFORM_EMPTY_BASES
#define PLATFORM_EMPTY_BASES
#endif