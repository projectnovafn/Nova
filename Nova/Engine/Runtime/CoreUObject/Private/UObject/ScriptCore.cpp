#include "UObject/Stack.h"
#include "Logging/LogMacros.h"

#include "KismetMemoryLibrary.h"

void UObject::ProcessEvent(UFunction* Function, uint8* Parms)
{
    static auto ProcessEvent = UKismetMemoryLibrary::Get<void (*)(UObject*, UFunction*, uint8_t*)>(L"UObject::ProcessEvent");

    return ProcessEvent(this, Function, Parms);
}

void UObject::ProcessEvent(const TCHAR* FunctionName, void* Parms)
{
    auto Function = this->FindFunction(FunctionName);
    
    if (!Function)
    {
#ifndef PRODUCTION
        UE_LOG(LogCore, Fatal, TEXT("ProcessEvent NULL: %s"), FunctionName);
#endif
        return;
    }

    ProcessEvent(Function, (uint8*)Parms);
}

void FFrame::Step(UObject* Context, RESULT_DECL)
{
    static auto Step = UKismetMemoryLibrary::Get<void (*)(FFrame*, UObject*, RESULT_DECL)>(L"FFrame::Step");

    Step(this, Context, RESULT_PARAM);
}

void FFrame::StepExplicitProperty(void* Result, UProperty* Property)
{
    static auto StepExplicitProperty = UKismetMemoryLibrary::Get<void (*)(FFrame*, void*, UProperty*)>(L"FFrame::StepExplicitProperty");

    StepExplicitProperty(this, Result, Property);
}