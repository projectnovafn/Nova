#include "UObject\Object.h"

#include "UObject\Package.h"
#include "UObject\Class.h"

#include "UObject\UnrealType.h"

#include "Templates\Casts.h"

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif

PropertyWrapper* UObject::GetClassProperty(const TCHAR* PropertyName)
{
    return this->GetClass()->GetProperty(PropertyName);
}

UField* UObject::GetClassField(const TCHAR* FieldName)
{
    return this->GetClass()->GetField(FieldName);
}

UFunction* UObject::FindFunction(const TCHAR* FunctionName)
{
    if (!this)
        return NULL;

    return Cast<UFunction>(this->GetClass()->GetField(FunctionName));
}

uint32 UObject::GetClassPropertyOffset(const TCHAR* PropertyName)
{
    return this->GetClassProperty(PropertyName)->OffsetInternal;
}

UClass* UObject::GetClass() const
{
    return this->Class;
}

UObject* UObject::GetOuter() const
{
    return this->Outer;
}

FName UObject::GetFName() const
{
    return this->Name;
}

FString UObject::GetPathName(const UObject* StopOuter)
{
    FString Result;
    GetPathName(StopOuter, Result);

    return Result;
}

void UObject::GetPathName(const UObject* StopOuter, FString& ResultString)
{
    if (this != StopOuter && this != NULL)
    {
        UObject* ObjOuter = Outer;

        if (ObjOuter && ObjOuter != StopOuter)
        {
            ObjOuter->GetPathName(StopOuter, ResultString);

            if (ObjOuter->Class != UPackage::StaticClass()
                && ObjOuter->Outer->Class == UPackage::StaticClass())
            {
                ResultString += SUBOBJECT_DELIMITER;
            }
            else
            {
                ResultString += TEXT(".");
            }
        }

        ResultString += Name.ToString();
    }
    else
    {
        ResultString += TEXT("None");
    }
}

FString UObject::GetFullName()
{
    FString Result;

    if (this != nullptr)
    {
        Result.Empty(128);
        Result += Class->Name.ToString();
        Result += TEXT(" ");

        GetPathName(NULL, Result);
    }
    else
    {
        Result += TEXT("None");
    }

    return Result;
}

FString UObject::GetName()
{
    if (this == NULL)
        return L"None";

	return Name.ToString();
}

void* UObject::GetInterfaceAddress(UClass* InterfaceClass)
{
    static auto GetInterfaceAddress = UKismetMemoryLibrary::Get<void* (*)(UObject*, UClass*)>(L"UObject::GetInterfaceAddress");

    return GetInterfaceAddress(this, InterfaceClass);
}

bool UObject::IsValid()
{
#ifdef PLATFORM_WINDOWS
    if (!this || IsBadReadPtr(this, 8))
        return false;

    if (!this->Class || IsBadReadPtr(this->Class, 8))
        return false;
#endif

    static auto IsValidLowLevelFast = UKismetMemoryLibrary::Get<bool (*)(UObject*, bool)>(L"UObject::IsValidLowLevelFast");

    return IsValidLowLevelFast(this, false);
}