#pragma once

#include "rtweekend.hpp"

template<typename T>
struct interval
{
    T min{+rt::infinity};
    T max{-rt::infinity};

    auto contains(const T t) const -> bool
    {
        return min <= t && t <= max;
    }

    auto surrounds(const T t) const -> bool
    {
        return min < t && t < max;
    }

    auto clamp(const T t) const -> T
    {
        if (t < min) {
            return min;
        } else if (t > max) {
            return max;
        } else {
            return t;
        }
    }

    static auto empty() -> interval<T> {
        return {};
    }

    static auto universe() -> interval<T> {
        return {-rt::infinity, +rt::infinity};
    }
}; 