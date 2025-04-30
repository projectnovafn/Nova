#include "UObject/NameTypes.h"
#include "CoreMinimal.h"

#include "KismetMemoryLibrary.h"

FName::FName(const TCHAR* Value, EFindName FindType)
{
    static auto Function = UKismetMemoryLibrary::Get<void (*)(const FName*, const TCHAR*, EFindName)>(L"FName::FName");
    Function(this, Value, FindType);
}

FString FName::ToString() const
{
    FString Result;

    static auto Function = UKismetMemoryLibrary::Get<void (*)(const FName*, FString*)>(L"FName::ToString");
    Function(this, &Result);

    return Result;
}