#pragma once

#include "Object.h"
#include "Script.h"
#include "Class.h"

class FFrame : public FOutputDevice
{
	GENERATED_USTRUCT_BODY(FFrame, CoreUObject)

public:
    UFunction* Node;
    UObject* Object;
    uint8_t* Code;
    uint8_t* Locals;

	UProperty* MostRecentProperty;
	uint8* MostRecentPropertyAddress;

	UPROPERTY_STRUCT(UField*, PropertyChainForCompiledIn)
	UField* PropertyChainForCompiledIn;

	void Step(UObject* Context, RESULT_DECL);
	void StepExplicitProperty(void* Result, UProperty* Property);

	template<class TProperty = UProperty>
	void StepCompiledIn(void* Result);

	template<class TProperty = UProperty, typename TNativeType>
	TNativeType& StepCompiledInRef(void* TemporaryBuffer);
};

template<class TProperty>
void FFrame::StepCompiledIn(void* Result)
{
	if (Code)
	{
		Step(Object, Result);
	}
	else
	{
		TProperty* Property = (TProperty*)PropertyChainForCompiledIn;
		PropertyChainForCompiledIn = Property->Next;
		StepExplicitProperty(Result, Property);
	}
}

template<class TProperty, typename TNativeType>
TNativeType& FFrame::StepCompiledInRef(void* TemporaryBuffer)
{
	MostRecentPropertyAddress = NULL;

	if (Code)
	{
		Step(Object, TemporaryBuffer);
	}
	else
	{
		checkSlow(dynamic_cast<TProperty*>(PropertyChainForCompiledIn) && dynamic_cast<UProperty*>(PropertyChainForCompiledIn));
		TProperty* Property = (TProperty*)PropertyChainForCompiledIn;
		PropertyChainForCompiledIn = Property->Next;
		StepExplicitProperty(TemporaryBuffer, Property);
	}

	return (MostRecentPropertyAddress != NULL) ? *(TNativeType*)(MostRecentPropertyAddress) : *(TNativeType*)TemporaryBuffer;
}