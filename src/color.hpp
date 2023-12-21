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

    auto r() const -> T 
    {
        return vec3<T>::e[0];
    }

    auto g() const -> T 
    {
        return vec3<T>::e[1];
    }

    auto b() const -> T 
    {
        return vec3<T>::e[2];
    }

    auto r() -> T &
    {
        return vec3<T>::e[0];
    }

    auto g() -> T &
    {
        return vec3<T>::e[1];
    }

    auto b() -> T &
    {
        return vec3<T>::e[2];
    }
};

template <typename T>
inline auto operator<<(std::ostream &out, color<T> pixel_color) -> std::ostream &
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
    return out;
}