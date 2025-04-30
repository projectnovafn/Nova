#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Templates/Casts.h"

class FField;

class UField : public UObject
{
    GENERATED_UCLASS_BODY(UField, CoreUObject)

public:

    UPROPERTY(UField*, Next)
    UField* Next;
};

class UStruct : public UField
{
    GENERATED_UCLASS_BODY(UStruct, CoreUObject)

public:

    UPROPERTY(UStruct*, SuperStruct)
    UStruct* SuperStruct;

    UPROPERTY(UField*, Children)
    UField* Children;

    UPROPERTY(int32, PropertiesSize)
    int32 PropertiesSize;

    UPROPERTY(FField*, ChildProperties)
    FField* ChildProperties;

    bool IsChildOf(UStruct* Struct);

    PropertyWrapper* GetProperty(const TCHAR* PropertyName);
    UField* GetField(const TCHAR* FieldName);

    uint32 GetPropertyOffset(const TCHAR* PropertyName);

    template <typename T>
    bool IsChildOf()
    {
        return IsChildOf(T::StaticClass());
    }
};

class UScriptStruct : public UStruct
{
    GENERATED_UCLASS_BODY(UScriptStruct, CoreUObject)

public:
};

class UFunction : public UStruct
{
    GENERATED_UCLASS_BODY(UFunction, CoreUObject)

public:

    UPROPERTY(void*, Func)
    void* Func;

    UPROPERTY(uint16, ParmsSize)
    uint16 ParmsSize;
};

class UClass : public UStruct
{
    GENERATED_UCLASS_BODY(UClass, CoreUObject)
private:

    UPROPERTY(UObject*, ClassDefaultObject)
    UObject* ClassDefaultObject;

public:

    template <typename T>
    T* GetDefaultObject()
    {
        return Cast<T>(this->ClassDefaultObject);
    }
};

template <typename T>
T* GetDefaultObject()
{
    return T::StaticClass() ? T::StaticClass()->GetDefaultObject<T>() : NULL;
}

#define GENERATED_USTRUCT_BODY(Class, Package) \
private: \
    static std::wstring GetRawStructName() { \
        return TEXT(#Class); \
    } \
    static std::wstring GetStructName() { \
        std::wstring StructName = TEXT(#Class); \
        return StructName.substr(1); \
    } \
    static std::wstring GetStructPackageName() { \
        return TEXT(#Package); \
    } \
    static std::wstring GetStructFullName() { \
        return L"/Script/" + GetStructPackageName() + L"." + GetStructName(); \
    } \
public: \
    static class UStruct* StaticStruct() { \
        static auto Class = FindObject<UStruct>(GetStructFullName().c_str()); \
		return Class; \
    } \
	static Class* Allocate() \
	{ \
		auto Data = (uint8_t*)FMemory::Malloc(StaticStruct()->PropertiesSize); \
        if (!Data) \
            return Allocate(); \
		FMemory::Memzero(Data, StaticStruct()->PropertiesSize); \
		return (Class*)Data; \
	} \
	Class* Copy() \
	{ \
		auto Data = Class::Allocate(); \
		FMemory::Memcpy(Data, this, StaticStruct()->PropertiesSize); \
		return (Class*)Data; \
	} \
	void CopyTo(Class* Data) \
	{ \
		FMemory::Memcpy(Data, this, StaticStruct()->PropertiesSize); \
	} \
private:

#define USTRUCT_COPYABLE(Class) \
    public: \
    Class(const Class& Other) { \
        FMemory::Memcpy((void*)this, (void*)&Other, StaticStruct()->PropertiesSize); \
    } private:

#define USTRUCT_ALLOCATABLE(Class) \
    public: \
    void* operator new(size_t size) { \
        return FMemory::Malloc(StaticStruct()->PropertiesSize); \
    } \
    void operator delete(void* ptr) { \
        FMemory::Free(ptr); \
    } private:

class FField
{
    GENERATED_USTRUCT_BODY(FField, CoreUObject)
public:

    UPROPERTY_STRUCT(FField*, Next)
    FField* Next;

    UPROPERTY_STRUCT(FName, Name)
    FName NamePrivate;

    FString GetName()
    {
        return NamePrivate.ToString();
    }

    FName GetFName()
    {
        return NamePrivate;
    }
};

class FieldWrapper
{
public:
    FField* NewField;
    UField* Field;

    FieldWrapper() { }

    FieldWrapper(FField* InNewField)
    {
        NewField = InNewField;
        Field = NULL;
    }

    FieldWrapper(UField* InField)
    {
        NewField = NULL;
        Field = InField;
    }

    inline FString GetName()
    {
        if (NewField)
            return NewField->GetName();

        if (Field)
            return Field->GetName();

        return FString();
    }

    inline void* InternalGetUnderlyingObject()
    {
        if (NewField)
            return NewField;

        if (Field)
            return Field;

        return NULL;
    }

    __declspec(property(get = InternalGetUnderlyingObject))
    void* UnderlyingObject;
};