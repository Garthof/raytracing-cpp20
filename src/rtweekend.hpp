#pragma once

#include <limits>
#include <memory>
#include <random>

namespace rt
{
using scalar_type = double;

constexpr auto infinity = std::numeric_limits<scalar_type>::infinity();
constexpr auto pi = 3.1415926535897932385;

template<typename T>
inline auto degrees_to_radians(const T degrees) -> T
{
    return degrees * pi / 180.0;
}

template<typename T>
inline auto random() -> T 
{
    constexpr auto min = static_cast<T>(0.);
    constexpr auto max = static_cast<T>(1.);
    static std::uniform_real_distribution<T> distribution{min, max};
    static std::mt19937 generator;
    return distribution(generator);
};

template<typename T>
inline auto random(const T min, const T max) -> T
{
    return min + (max - min) * random<T>();
}
} // namespace rt