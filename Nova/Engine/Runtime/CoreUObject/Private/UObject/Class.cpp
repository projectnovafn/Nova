#include "UObject/Class.h"
#include "CoreUObject.h"
#include "Engine/World.h"

#include "Kismet/KismetSystemLibrary.h"

bool UObject::IsA(UClass* ToCheck)
{
    for (auto Super = this->Class; Super; Super = Cast<UClass>(Super->SuperStruct))
    {
        if (Super == ToCheck)
        {
            return true;
        }
    }

    return false;
}

UObject* UObject::GetTypedOuter(UClass* Target)
{
    UObject* Result = NULL;
    for (UObject* NextOuter = GetOuter(); Result == NULL && NextOuter != NULL; NextOuter = NextOuter->GetOuter())
    {
        if (NextOuter->IsA(Target))
        {
            Result = NextOuter;
        }
    }

    return Result;
}

UWorld* UObject::GetWorld()
{
    return GWorld;
}

bool UStruct::IsChildOf(UStruct* Struct)
{
    for (auto Super = this; Super; Super = Cast<UClass>(Super->SuperStruct))
    {
        if (Super == Struct)
        {
            return true;
        }
    }

    return false;
}


PropertyWrapper* UStruct::GetProperty(const TCHAR* PropertyName)
{
    auto PropertyFName = FName(PropertyName);

    for (auto Super = this; Super; Super = Super->SuperStruct)
    {
        for (auto Next = Super->Children; Next; Next = Next->Next)
        {
            if (Next->GetFName() == PropertyFName)
            {
                return new PropertyWrapper(Cast<UProperty>(Next));
            }
        }

        if (UKismetSystemLibrary::GetEngineVersionNum() >= 4.25)
        {
            for (auto Property = Super->ChildProperties; Property; Property = Property->Next)
            {
                if (Property->GetFName() == PropertyFName)
                {
                    return new PropertyWrapper((FProperty*)Property);
                }
            }
        }
    }

#ifdef STAGING
    UE_LOG(LogCore, Error, TEXT("%s: MISSING PROPERTY %s"), __FUNCTIONW__, PropertyName);
#endif

    return NULL;
}

UField* UStruct::GetField(const TCHAR* FieldName)
{
    auto FieldFName = FName(FieldName);

    for (auto Super = this; Super; Super = Super->SuperStruct)
    {
        for (auto Next = Super->Children; Next; Next = Next->Next)
        {
            if (Next->GetFName() == FieldFName)
            {
                return Cast<UField>(Next);
            }
        }
    }

#ifdef STAGING
    UE_LOG(LogCore, Error, TEXT("%s: MISSING FIELD %s"), __FUNCTIONW__, FieldName);
#endif

    return NULL;
}

uint32 UStruct::GetPropertyOffset(const TCHAR* PropertyName)
{
    if (auto Property = this->GetProperty(PropertyName))
    {
        return Property->OffsetInternal;
    }

    return -1;
}