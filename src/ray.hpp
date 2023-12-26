#pragma once

#include "vec3.hpp"

template<typename T>
struct ray
{
    coord<T> origin;
    vec3<T> direction;

    auto at(T t) const -> coord<T>
    {
        return static_cast<coord<T>>(origin + t * direction);
    }
};