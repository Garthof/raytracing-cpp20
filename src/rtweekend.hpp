#pragma once

#include <limits>
#include <memory>

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
} // namespace rt