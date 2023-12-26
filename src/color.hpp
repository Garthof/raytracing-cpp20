#pragma once

#include <cmath>

#include <iostream>

#include "interval.hpp"
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

template<typename T>
struct pixel
{
    color<T> pixel_color;
    int samples_per_pixel; 
};

template <typename T>
inline auto operator<<(std::ostream &t_out, const pixel<T> t_pixel) -> std::ostream &
{
    // Divide the color by the number of samples
    const auto norm_color = static_cast<color<T>>(t_pixel.pixel_color / static_cast<T>(t_pixel.samples_per_pixel));

    // Transform color from linear to gamma space
    constexpr auto linear_to_gamma = [](const auto linear_color) -> color<T> {
        return {std::sqrt(linear_color.r()), std::sqrt(linear_color.g()), std::sqrt(linear_color.b())};
    };
    const auto gamma_color = linear_to_gamma(norm_color);

    // Write the translated [0,255] value of each color component.
    constexpr auto intensity = interval<T>{0.000, 0.999};
    t_out 
        << static_cast<int>(256 * intensity.clamp(gamma_color.r())) << ' '
        << static_cast<int>(256 * intensity.clamp(gamma_color.g())) << ' '
        << static_cast<int>(256 * intensity.clamp(gamma_color.b())) << '\n';
    return t_out;
}