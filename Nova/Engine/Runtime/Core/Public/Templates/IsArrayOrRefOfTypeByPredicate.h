#pragma once

#include "CoreTypes.h"

template <typename T, template <typename> class Predicate>
struct TIsArrayOrRefOfTypeByPredicate
{
	enum { Value = false };
};

template <typename ArrType, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<               ArrType[], Predicate> { enum { Value = Predicate<ArrType>::Value }; };
template <typename ArrType, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<const          ArrType[], Predicate> { enum { Value = Predicate<ArrType>::Value }; };
template <typename ArrType, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<      volatile ArrType[], Predicate> { enum { Value = Predicate<ArrType>::Value }; };
template <typename ArrType, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<const volatile ArrType[], Predicate> { enum { Value = Predicate<ArrType>::Value }; };

template <typename ArrType, unsigned int N, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<               ArrType[N], Predicate> { enum { Value = Predicate<ArrType>::Value }; };
template <typename ArrType, unsigned int N, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<const          ArrType[N], Predicate> { enum { Value = Predicate<ArrType>::Value }; };
template <typename ArrType, unsigned int N, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<      volatile ArrType[N], Predicate> { enum { Value = Predicate<ArrType>::Value }; };
template <typename ArrType, unsigned int N, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<const volatile ArrType[N], Predicate> { enum { Value = Predicate<ArrType>::Value }; };

template <typename ArrType, unsigned int N, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<               ArrType(&)[N], Predicate> { enum { Value = Predicate<ArrType>::Value }; };
template <typename ArrType, unsigned int N, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<const          ArrType(&)[N], Predicate> { enum { Value = Predicate<ArrType>::Value }; };
template <typename ArrType, unsigned int N, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<      volatile ArrType(&)[N], Predicate> { enum { Value = Predicate<ArrType>::Value }; };
template <typename ArrType, unsigned int N, template <typename> class Predicate> struct TIsArrayOrRefOfTypeByPredicate<const volatile ArrType(&)[N], Predicate> { enum { Value = Predicate<ArrType>::Value }; };