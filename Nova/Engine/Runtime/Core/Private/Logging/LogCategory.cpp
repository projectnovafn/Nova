#include "Logging/LogCategory.h"

#include "Containers/UnrealString.h"
#include "CoreGlobals.h"
#include "Misc/AssertionMacros.h"

FLogCategoryBase::FLogCategoryBase(const FLogCategoryName& InCategoryName, ELogVerbosity::Type InDefaultVerbosity, ELogVerbosity::Type InCompileTimeVerbosity)
	: DefaultVerbosity(InDefaultVerbosity)
	, CompileTimeVerbosity(InCompileTimeVerbosity)
	, CategoryName(InCategoryName)
{
	checkSlow(!(Verbosity & ELogVerbosity::BreakOnLog));
}

FLogCategoryBase::~FLogCategoryBase()
{
	checkSlow(!(Verbosity & ELogVerbosity::BreakOnLog));
}