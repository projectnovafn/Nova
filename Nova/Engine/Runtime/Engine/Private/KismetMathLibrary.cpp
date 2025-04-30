#include "Kismet/KismetMathLibrary.h"

float UKismetMathLibrary::RandomFloatInRange(float Min, float Max)
{
    struct
    {
        float Min;
        float Max;
        float ReturnValue;
    } params(Min, Max);

    GetDefaultObject<UKismetMathLibrary>()->ProcessEvent(L"RandomFloatInRange", &params);

    return params.ReturnValue;
}

int UKismetMathLibrary::RandomIntegerInRange(int Min, int Max)
{
    struct
    {
        int Min;
        int Max;
        int ReturnValue;
    } params(Min, Max);

    GetDefaultObject<UKismetMathLibrary>()->ProcessEvent(L"RandomIntegerInRange", &params);
    
    return params.ReturnValue;
}

FRotator UKismetMathLibrary::MakeRotFromX(FVector X)
{
    struct
    {
        FVector X;
        FRotator ReturnValue;
    } params(X);

    GetDefaultObject<UKismetMathLibrary>()->ProcessEvent(L"MakeRotFromX", &params);

    return params.ReturnValue;
}

FVector UKismetMathLibrary::InverseTransformDirection(FTransform T, FVector Direction)
{
    struct
    {
        FTransform T;
        FVector Direction;
        FVector ReturnValue;
    } params(T, Direction);

    GetDefaultObject<UKismetMathLibrary>()->ProcessEvent(L"InverseTransformDirection", &params);

    return params.ReturnValue;
}

FVector UKismetMathLibrary::InverseTransformLocation(FTransform T, FVector Location)
{
    struct
    {
        FTransform T;
        FVector Location;
        FVector ReturnValue;
    } params(T, Location);

    GetDefaultObject<UKismetMathLibrary>()->ProcessEvent(L"InverseTransformLocation", &params);

    return params.ReturnValue;
}

FRotator UKismetMathLibrary::Quat_Rotator(FQuat Q)
{
    struct
    {
        FQuat Q;
        FRotator ReturnValue;
    } params(Q);

    GetDefaultObject<UKismetMathLibrary>()->ProcessEvent(L"Quat_Rotator", &params);

    return params.ReturnValue;
}