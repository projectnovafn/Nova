#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"
#include "UObjectGlobals.h"

class UWorld;
class FieldWrapper;
class PropertyWrapper;

#define GENERATED_UCLASS_BODY(Class, Package) \
private: \
    static constexpr std::wstring GetRawStructName() { \
        return TEXT(#Class); \
    } \
    static constexpr std::wstring GetStructName() { \
        std::wstring StructName = TEXT(#Class); \
        return StructName.substr(1); \
    } \
    static constexpr std::wstring GetStructPackageName() { \
        return TEXT(#Package); \
    } \
    static constexpr std::wstring GetStructFullName() { \
        return L"/Script/" + GetStructPackageName() + L"." + GetStructName(); \
    } \
public: \
    static class UClass* StaticClass() { \
        static auto ClassObject = (UClass*)StaticFindObject(GetStructFullName().c_str()); \
		return ClassObject; \
    } \
private: \

#define UPROPERTY(Type, Name) \
    Type& InternalGet##Name() { \
		static auto Offset = UKismetMemoryLibrary::Get((GetRawStructName() + L"::" + TEXT(#Name)).c_str()); \
		if (Offset == -1) Offset = this->GetClassPropertyOffset(TEXT(#Name)); \
        return *(Type*)(int64(this) + Offset); \
    } \
    void InternalSet##Name(Type Value) { \
        static auto Offset = UKismetMemoryLibrary::Get((GetRawStructName() + L"::" + TEXT(#Name)).c_str()); \
		if (Offset == -1) Offset = this->GetClassPropertyOffset(TEXT(#Name)); \
		*(Type*)(int64(this) + Offset) = Value; \
    } \
	 __declspec(property(get = InternalGet##Name, put = InternalSet##Name))

#define UPROPERTY_PTR(Type, Name) \
    Type* InternalGet##Name() { \
        static auto Offset = this->GetClassPropertyOffset(TEXT(#Name)); \
        return (Type*)(int64(this) + Offset); \
    } \
    void InternalSet##Name(Type* Value) { \
		FMemory::Memcpy(InternalGet##Name(), Value, Type::StaticStruct()->PropertiesSize);\
    } \
	 __declspec(property(get = InternalGet##Name, put = InternalSet##Name))

#define UPROPERTY_STRUCT(Type, Name) \
    Type& InternalGet##Name() { \
        static auto Offset = UKismetMemoryLibrary::Get((GetRawStructName() + L"::" + TEXT(#Name)).c_str()); \
		if (Offset == -1) Offset = StaticStruct()->GetPropertyOffset(TEXT(#Name)); \
        return *(Type*)(int64(this) + Offset); \
    } \
    void InternalSet##Name(Type Value) { \
        static auto Offset = UKismetMemoryLibrary::Get((GetRawStructName() + L"::" + TEXT(#Name)).c_str()); \
		if (Offset == -1) Offset = StaticStruct()->GetPropertyOffset(TEXT(#Name)); \
		*(Type*)(int64(this) + Offset) = Value; \
    } \
	 __declspec(property(get = InternalGet##Name, put = InternalSet##Name))

#define UPROPERTY_STRUCT_PTR(Type, Name) \
    Type* InternalGet##Name() { \
        static auto Offset = StaticStruct()->GetPropertyOffset(TEXT(#Name)); \
        return (Type*)(int64(this) + Offset); \
    } \
    void InternalSet##Name(Type* Value) { \
		FMemory::Memcpy(InternalGet##Name(), Value, Type::StaticStruct()->PropertiesSize);\
    } \
	 __declspec(property(get = InternalGet##Name, put = InternalSet##Name))


#define UPROPERTY_NOTIFY(Type, Name) \
    Type& InternalGet##Name() { \
		static auto Offset = this->GetClassPropertyOffset(TEXT(#Name)); \
        return *(Type*)(int64(this) + Offset); \
    } \
    void InternalSet##Name(Type Value) { \
		static auto Offset = this->GetClassPropertyOffset(TEXT(#Name)); \
		*(Type*)(int64(this) + Offset) = Value; \
		this->OnRep_##Name(); \
    } \
	void OnRep_##Name() { \
		this->ProcessEvent((std::wstring(L"OnRep_") + TEXT(#Name)).c_str()); \
	} \
	 __declspec(property(get = InternalGet##Name, put = InternalSet##Name))

#define UPROPERTY_BOOL(Name) \
    bool InternalGet##Name() { \
        static auto Property = (BoolPropertyWrapper*)this->GetClassProperty(TEXT(#Name)); \
		return Property->GetPropertyValue(this); \
    } \
    void InternalSet##Name(bool Value) { \
        static auto Property = (BoolPropertyWrapper*)this->GetClassProperty(TEXT(#Name)); \
		Property->SetPropertyValue(this, Value); \
    } \
	 __declspec(property(get = InternalGet##Name, put = InternalSet##Name))

#define UPROPERTY_BOOL_NOTIFY(Name) \
    bool InternalGet##Name() { \
        static auto Property = (BoolPropertyWrapper*)this->GetClassProperty(TEXT(#Name)); \
		return Property->GetPropertyValue(this); \
    } \
    void InternalSet##Name(bool Value) { \
        static auto Property = (BoolPropertyWrapper*)this->GetClassProperty(TEXT(#Name)); \
		Property->SetPropertyValue(this, Value); \
		this->OnRep_##Name(); \
    } \
	void OnRep_##Name() { \
		this->ProcessEvent((std::wstring(L"OnRep_") + TEXT(#Name)).c_str()); \
	} \
	 __declspec(property(get = InternalGet##Name, put = InternalSet##Name))

#define UPROPERTY_STRUCT_BOOL(Name) \
    bool InternalGet##Name() { \
        static auto Property = (BoolPropertyWrapper*)StaticStruct()->GetProperty(TEXT(#Name)); \
		return Property->GetPropertyValue(this); \
    } \
    void InternalSet##Name(bool Value) { \
        static auto Property = (BoolPropertyWrapper*)StaticStruct()->GetProperty(TEXT(#Name)); \
		Property->SetPropertyValue(this, Value); \
    } \
	 __declspec(property(get = InternalGet##Name, put = InternalSet##Name))

class UObject
{
	GENERATED_UCLASS_BODY(UObject, CoreUObject)

public:
	void** VTable;

private:

	EObjectFlags ObjectFlags;

public:

	int32 Index;

// private:

	UClass* Class;
	FName Name;
	UObject* Outer;

public:

	PropertyWrapper* GetClassProperty(const TCHAR* PropertyName);
	UField* GetClassField(const TCHAR* FieldName);
	uint32_t GetClassPropertyOffset(const TCHAR* PropertyName);

	UFunction* FindFunction(const TCHAR* FunctionName);

	UClass* GetClass() const;
	UObject* GetOuter() const;
	FName GetFName() const;

	FString GetPathName(const UObject* StopOuter = nullptr);
	void GetPathName(const UObject* StopOuter, FString& resultString);

	FString GetFullName();
	FString GetName();

	void ProcessEvent(UFunction* Function, uint8* Parms);
	void ProcessEvent(const TCHAR* FunctionName, void* Parms = NULL);

	bool IsA(UClass* Class);
	UObject* GetTypedOuter(UClass* Target);

	UWorld* GetWorld();
	void* GetInterfaceAddress(UClass* InterfaceClass);

	bool IsValid();

	FORCEINLINE bool HasAnyFlags(EObjectFlags FlagsToCheck)
	{
		return (ObjectFlags & FlagsToCheck) != 0;
	}

	FORCEINLINE bool HasAllFlags(EObjectFlags FlagsToCheck)
	{
		return (ObjectFlags & FlagsToCheck) == FlagsToCheck;
	}

	template <class T>
	FORCEINLINE bool IsA()
	{
		return IsA(T::StaticClass());
	}

	template <typename T>
	FORCEINLINE T* GetTypedOuter()
	{
		return (T*)GetTypedOuter(T::StaticClass());
	}
};