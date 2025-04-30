#pragma once

#include "CoreTypes.h"
#include "Misc/OutputDeviceConsole.h"
#include "UObject/NameTypes.h"

class FWindowsConsoleOutputDevice : public FOutputDeviceConsole
{
private:
	Windows::HANDLE ConsoleHandle;

	bool OverrideColorSet;
	bool bIsAttached;

	virtual void LogInternal(const TCHAR* Message, va_list args);

public:

	FWindowsConsoleOutputDevice();

	virtual void Show(bool ShowWindow);

	virtual bool IsShown();

	virtual bool IsAttached();

	virtual bool CanBeUsedOnAnyThread() const override;
	virtual bool CanBeUsedOnPanicThread() const override;

	virtual void Serialize(const TCHAR* Data, ELogVerbosity::Type Verbosity, const TCHAR* Category, const double Time) override;
	virtual void Serialize(const TCHAR* Data, ELogVerbosity::Type Verbosity, const TCHAR* Category) override;

	void SetColor(const TCHAR* Color);
};