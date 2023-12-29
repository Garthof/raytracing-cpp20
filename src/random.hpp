#pragma once

#include <random>
#include <stdexcept>

#include "vec3.hpp"

namespace rt
{
template<typename T>
inline auto random_t(const T min = 0., const T max = 1.) -> T
{
    static std::uniform_real_distribution<T> distribution{min, max};
    static std::mt19937 generator;
    return distribution(generator);
}

template<typename T>
inline auto random_v(const T min = 0., const T max = 1.) -> vec3<T>
{
    return {random_t(min, max), random_t(min, max), random_t(min, max)};
}    

template<typename T>
inline auto random_vec_in_unit_sphere() -> vec3<T>
{
    while (true) {
        const auto p = random_v(-1., +1.);
        if (p.length_squared() < 1.) {
            return p;
        }
    }

    throw std::logic_error{"random_in_unit_sphere: vec not found"};
}

template<typename T>
inline auto random_unit_vec_on_sphere() -> vec3<T>
{
    return random_vec_in_unit_sphere<T>().unit_vector();
}

template<typename T>
inline auto random_unit_vec_on_hemisphere(vec3<T> normal) -> vec3<T>
{
    const auto vec_on_sphere = random_unit_vec_on_sphere<T>();
    return dot(vec_on_sphere, std::move(normal)) > 0.
            ? vec_on_sphere
            : -vec_on_sphere; 
}

template<typename T>
inline auto random_vec_in_unit_disk() -> vec3<T>
{
    while (true) {
        const auto p = vec3{random_t(-1., +1.), random_t(-1., +1.), 0.};
        if (p.length_squared() < 1.) {
            return p;
        }
    }

    throw std::logic_error{"random_in_unit_disc: vec not found"};
}
} // namespace rt
 