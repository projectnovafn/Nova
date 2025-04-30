#pragma once
#include "CoreMinimal.h"

enum class EDetourType
{
	Detour,
	Rel32Call,
	VFSwap
};

enum class EReflectedDetourType
{
	VFSwap,
	ExecSwap,
	ImplSwap,
	ImplDetour,
	CallDetour
};

class UKismetDetoursLibrary
{
private:

	static void AddHookInternal(void** Target, void* Detour, EDetourType Type);
	static void RemoveHookInternal(void** Target, void* Detour, EDetourType Type);

	static bool AddReflectedDetourInternal(UObject* Object, UFunction* Function, EReflectedDetourType Type, void* Detour, void** Original, int Skip, const std::vector<uint8_t>& ScanBytes);

public:

	static void AddHook(void** Target, void* Detour, EDetourType Type)
	{
		return AddHookInternal(Target, Detour, Type);
	}

	static void RemoveHook(void** Target, void* Detour, EDetourType Type)
	{
		return RemoveHookInternal(Target, Detour, Type);
	}

	template <typename TargetType, typename DetourType>
	static void AddHook(TargetType& Target, DetourType Detour, EDetourType Type = EDetourType::Detour)
	{
		return AddHookInternal((void**)&Target, (void**)Detour, Type);
	}

	template <typename TargetType, typename DetourType>
	static void RemoveHook(TargetType& Target, DetourType Detour, EDetourType Type = EDetourType::Detour)
	{
		return RemoveHookInternal((void**)&Target, (void**)Detour, Type);
	}

	static void BeginTransaction();
	static void CommitTransaction();

	template <typename ObjectType, typename DetourType, typename OriginalType>
	static bool AddReflectedDetour(ObjectType* Object, const TCHAR* FunctionName, EReflectedDetourType Type, DetourType Detour, OriginalType& Original = NULL, int Skip = 0, const std::vector<uint8_t>& ScanBytes = {})
	{
		return AddReflectedDetourInternal(Object, Object->FindFunction(FunctionName), Type, (void*)Detour, (void**)&Original, Skip, ScanBytes);
	}

	template <typename ObjectType, typename DetourType, typename OriginalType>
	static bool AddReflectedDetour(const TCHAR* FunctionName, EReflectedDetourType Type, DetourType Detour, OriginalType& Original = NULL, int Skip = 0, const std::vector<uint8_t>& ScanBytes = {})
	{
		auto DefaultObject = GetDefaultObject<ObjectType>();

		if (DefaultObject == NULL)
			return false;

		return AddReflectedDetourInternal(DefaultObject, DefaultObject->FindFunction(FunctionName), Type, (void*)Detour, (void**)&Original, Skip, ScanBytes);
	}
};

class UDetour
{
	void** Target;
	void* Detour;
	EDetourType Type;

public:

	template <typename TargetType, typename DetourType>
	void Init(TargetType& InTarget, DetourType InDetour, EDetourType InType = EDetourType::Detour)
	{
		Target = (void**)&InTarget;
		Detour = (void*)InDetour;
		Type = InType;
	}

	void Commit()
	{
		UKismetDetoursLibrary::BeginTransaction();
		UKismetDetoursLibrary::AddHook(Target, Detour, Type);
		UKismetDetoursLibrary::CommitTransaction();
	}

	void Remove()
	{
		UKismetDetoursLibrary::BeginTransaction();
		UKismetDetoursLibrary::RemoveHook(Target, Detour, Type);
		UKismetDetoursLibrary::CommitTransaction();
	}
};