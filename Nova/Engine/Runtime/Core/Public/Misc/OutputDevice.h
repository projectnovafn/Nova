#pragma once

#include "CoreFwd.h"
#include "CoreTypes.h"
#include "Logging/LogVerbosity.h"
#include "Misc/VarArgs.h"
#include "Templates/AndOrNot.h"
#include "Templates/IsArrayOrRefOfTypeByPredicate.h"
#include "Templates/IsValidVariadicFunctionArg.h"
#include "Traits/IsCharEncodingCompatibleWith.h"

class FString;
class FText;

#ifndef USE_DEBUG_LOGGING
#define USE_DEBUG_LOGGING 1
#endif

#if !PLATFORM_SUPPORTS_COLORIZED_OUTPUT_DEVICE

#define SET_WARN_COLOR(Color)
#define SET_WARN_COLOR_AND_BACKGROUND(Color, Bkgrnd)
#define CLEAR_WARN_COLOR() 
#else

namespace OutputDeviceColor
{
	inline const TCHAR* const COLOR_BLACK = TEXT("0000");

	inline const TCHAR* const COLOR_DARK_RED = TEXT("1000");
	inline const TCHAR* const COLOR_DARK_GREEN = TEXT("0100");
	inline const TCHAR* const COLOR_DARK_BLUE = TEXT("0010");
	inline const TCHAR* const COLOR_DARK_YELLOW = TEXT("1100");
	inline const TCHAR* const COLOR_DARK_CYAN = TEXT("0110");
	inline const TCHAR* const COLOR_DARK_PURPLE = TEXT("1010");
	inline const TCHAR* const COLOR_DARK_WHITE = TEXT("1110");
	inline const TCHAR* const COLOR_GRAY = COLOR_DARK_WHITE;

	inline const TCHAR* const COLOR_RED = TEXT("1001");
	inline const TCHAR* const COLOR_GREEN = TEXT("0101");
	inline const TCHAR* const COLOR_BLUE = TEXT("0011");
	inline const TCHAR* const COLOR_YELLOW = TEXT("1101");
	inline const TCHAR* const COLOR_CYAN = TEXT("0111");
	inline const TCHAR* const COLOR_PURPLE = TEXT("1011");
	inline const TCHAR* const COLOR_WHITE = TEXT("1111");

	inline const TCHAR* const COLOR_NONE = TEXT("");
}

using namespace OutputDeviceColor;

#ifndef SET_WARN_COLOR

#define SET_WARN_COLOR(Color) \
	UE_LOG(LogHAL, SetColor, TEXT("%s"), Color);
#define SET_WARN_COLOR_AND_BACKGROUND(Color, Bkgrnd) \
	UE_LOG(LogHAL, SetColor, TEXT("%s%s"), Color, Bkgrnd);

#define CLEAR_WARN_COLOR() \
	UE_LOG(LogHAL, SetColor, TEXT("%s"), COLOR_NONE);

#endif
#endif

namespace ELogTimes
{
	enum Type
	{
		None,
		UTC,
		Local
	};
}

class FName;

class FOutputDevice
{
public:
	FOutputDevice()
		: bSuppressEventTag(false)
		, bAutoEmitLineTerminator(true)
	{}

	FOutputDevice(FOutputDevice&&) = default;
	FOutputDevice(const FOutputDevice&) = default;
	FOutputDevice& operator=(FOutputDevice&&) = default;
	FOutputDevice& operator=(const FOutputDevice&) = default;

	virtual ~FOutputDevice() = default;

	virtual void Flush()
	{
	}

	virtual void TearDown()
	{
	}

	void SetSuppressEventTag(bool bInSuppressEventTag)
	{
		bSuppressEventTag = bInSuppressEventTag;
	}
	FORCEINLINE bool GetSuppressEventTag() const { return bSuppressEventTag; }
	void SetAutoEmitLineTerminator(bool bInAutoEmitLineTerminator)
	{
		bAutoEmitLineTerminator = bInAutoEmitLineTerminator;
	}
	FORCEINLINE bool GetAutoEmitLineTerminator() const { return bAutoEmitLineTerminator; }

	virtual void Dump(class FArchive& Ar)
	{
	}

	virtual bool IsMemoryOnly() const
	{
		return false;
	}

	virtual bool CanBeUsedOnAnyThread() const
	{
		return false;
	}

	virtual bool CanBeUsedOnMultipleThreads() const
	{
		return false;
	}

	virtual bool CanBeUsedOnPanicThread() const
	{
		return false;
	}

protected:
	bool bSuppressEventTag;
	bool bAutoEmitLineTerminator;
};