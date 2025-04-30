#pragma once

#include "CoreTypes.h"
#include "Misc/OutputDevice.h"
#include "Containers/UnrealString.h"

class FOutputDeviceConsole
	: public FOutputDevice
{
private:

	virtual void LogInternal(const TCHAR* Message, va_list args) = 0;

public:

	virtual void Show(bool ShowWindow) = 0;
	virtual bool IsShown() = 0;

	void Log(const TCHAR* Message, ...)
	{
		va_list args;
		va_start(args, Message);
		LogInternal(Message, args);
		va_end(args);
	}
		
	virtual bool IsAttached()
	{
		return false;
	}

	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const TCHAR* Category) = 0;
	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const TCHAR* Category, const double Time)
	{
		Serialize(V, Verbosity, Category);
	}
};