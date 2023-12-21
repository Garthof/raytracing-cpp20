#pragma once

#include "rtweekend.hpp"

template<typename T>
struct interval
{
    T min{+rt::infinity};
    T max{-rt::infinity};

    bool contains(const T t) const 
    {
        return min <= t && t <= max;
    }

    bool surrounds(const T t) const 
    {
        return min < t && t < max;
    }

    static auto empty() -> interval<T> {
        return {};
    }

    static auto universe() -> interval<T> {
        return {-rt::infinity, +rt::infinity};
    }
};