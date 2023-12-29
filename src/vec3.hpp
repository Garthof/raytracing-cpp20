#pragma once

#include <cmath>

#include <algorithm>
#include <array>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <optional>
#include <type_traits>
#include <stdexcept>

#include "rtweekend.hpp"

template <typename T>
concept IsScalar = std::is_scalar_v<T>;

template <IsScalar T> 
class vec3 
{
public:
    using array_type = std::array<T, 3>;
    using index_type = array_type::size_type;

    array_type e;

    vec3(): vec3{T{}, T{}, T{}} {}

    vec3(std::initializer_list<T> l) 
    {
        if (l.size() > 3) {
            throw std::invalid_argument{"Too many arguments"};
        }
        std::copy(l.begin(), l.end(), e.begin());
    }

    virtual ~vec3() = default;

    auto x() const -> T
    {
        return vec3::e[0];
    }

    auto y() const -> T
    {
        return vec3::e[1];
    }

    auto z() const -> T
    {
        return vec3::e[2];
    }

    auto x() -> T &
    {
        return vec3::e[0];
    }

    auto y() -> T &
    {
        return vec3<T>::e[1];
    }

    auto z() -> T &
    {
        return vec3::e[2];
    }

    auto operator[](index_type i) const -> T
    {
        return e[i];
    }

    auto operator[](index_type i) -> T & 
    {
        return e[i];
    }

    auto operator-() const -> vec3
    { 
        return {-e[0], -e[1], -e[2]}; 
    }

    auto operator+=(const vec3 v) -> vec3 & 
    {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }

    auto operator-=(const vec3 v) -> vec3 & 
    {
        *this += -v;
        return *this;
    }

    auto operator*=(const vec3<T> v) -> vec3 &
    {
        e[0] *= v[0];
        e[1] *= v[1];
        e[2] *= v[2];
        return *this;
    }

    template<IsScalar U>
    auto operator*=(const U u) -> vec3 &
    {
        e[0] *= static_cast<T>(u);
        e[1] *= static_cast<T>(u);
        e[2] *= static_cast<T>(u);
        return *this;
    }

    template<IsScalar U>
    auto operator/=(const U u) -> vec3 & 
    {
        return *this *= static_cast<T>(1) / static_cast<T>(u);
    }

    auto length() const -> T 
    {
        return std::sqrt(length_squared());
    }

    auto length_squared() const -> T 
    {
        return std::accumulate(e.begin(), e.end(), T{}, 
                               [](const T a, const T b) { return a + b * b; });
    }

    auto unit_vector() const -> vec3
    {
        return *this / length();
    }

    /// @return true if the vector is close to zero in all dimensions 
    auto near_zero(const T eps = 1e-8) const -> bool
    {
        const auto abs_near_zero = [&](const auto value) {
            return std::fabs(value) < eps;
        };
        return abs_near_zero(e[0]) && abs_near_zero(e[1]) && abs_near_zero(e[2]);
    }
};

template <typename T>
inline auto operator<<(std::ostream &out, const vec3<T> v) -> std::ostream &
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

template <typename T>
inline auto operator+(vec3<T> lhs, const vec3<T> rhs) -> vec3<T> 
{
    lhs += rhs;
    return lhs;
}

template <typename T>
inline auto operator-(vec3<T> lhs, const vec3<T> rhs) -> vec3<T> 
{
    lhs -= rhs;
    return lhs;
}

template <typename T>
inline auto operator*(vec3<T> lhs, const vec3<T> rhs) -> vec3<T> 
{
    lhs *= rhs;
    return lhs;
}

template <typename T, IsScalar U>
inline auto operator*(const U u, vec3<T> v) -> vec3<T> 
{
    v *= u;
    return v;
}

template <typename T, IsScalar U>
inline auto operator*(vec3<T> v, const U u) -> vec3<T> 
{
    return u * std::move(v);
}

template <typename T, typename U>
inline auto operator/(vec3<T> v, const U u) -> vec3<T> 
{
    v /= u;
    return v;
}

template <typename T>
inline auto dot(const vec3<T> lhs, const vec3<T> rhs) -> T 
{
    const auto product = lhs * rhs;
    return std::accumulate(product.e.begin(), product.e.end(), T{});
}

template <typename T>
inline auto cross(const vec3<T> u, const vec3<T> v) -> vec3<T> 
{
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
            u.e[2] * v.e[0] - u.e[0] * v.e[2],
            u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

template <typename T>
inline auto reflect(const vec3<T> v, const vec3<T> n) -> vec3<T>
{
    return v - 2 * dot(v, n) * n;
}

template <typename T>
inline auto refract(const vec3<T> uv, const vec3<T> n, const double etai_over_etat) -> std::optional<vec3<T>>
{
    const auto cos_theta = std::min(dot(-uv, n), 1.);
    const auto sin_theta = std::sqrt(1. - cos_theta * cos_theta);

    const auto can_refract = etai_over_etat * sin_theta <= 1.;
    if (can_refract) {
        const auto r_out_perp = etai_over_etat * (uv + cos_theta * n);
        const auto r_out_parallel = -std::sqrt(std::abs(1. - r_out_perp.length_squared())) * n;
        return r_out_perp + r_out_parallel;
    }

    return std::nullopt;
}

template <typename T>
class coord : public vec3<T>
{
public:
    coord() = default;
    coord(std::initializer_list<T> l) : vec3<T>{l} {}
    explicit coord(vec3<T> v) : vec3<T>{std::move(v)} {};
};