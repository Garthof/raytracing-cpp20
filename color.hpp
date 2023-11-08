#pragma once

#include <iostream>

#include "vec3.hpp"

template <typename T>
class color : public vec3<T>
{
public:
    color() = default;
    color(std::initializer_list<T> l) : vec3<T>{l} {}
    explicit color(vec3<T> v) : vec3<T>{std::move(v)} {};

    T r() const
    {
        return vec3<T>::e[0];
    }

    T g() const
    {
        return vec3<T>::e[1];
    }

    T b() const 
    {
        return vec3<T>::e[2];
    }

    T &r()
    {
        return vec3<T>::e[0];
    }

    T &g()
    {
        return vec3<T>::e[1];
    }

    T &b()
    {
        return vec3<T>::e[2];
    }
};

template <typename T>
inline std::ostream &operator<<(std::ostream &out, color<T> pixel_color) 
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
    return out;
}