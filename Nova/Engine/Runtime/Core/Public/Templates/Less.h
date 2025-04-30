#pragma once

/**
 * Binary predicate class for sorting elements in order.  Assumes < operator is defined for the template type.
 * Forward declaration exists in ContainersFwd.h
 *
 * See: http://en.cppreference.com/w/cpp/utility/functional/less
 */
template <typename T >
struct TLess
{
    FORCEINLINE bool operator()(const T& A, const T& B) const
    {
        return A < B;
    }
};

template <>
struct TLess<void>
{
    template <typename T>
    FORCEINLINE bool operator()(const T& A, const T& B) const
    {
        return A < B;
    }
};
