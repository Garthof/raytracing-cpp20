#pragma once

#include <cmath>

#include <array>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <stdexcept>

template <typename T> 
class vec3 
{
public:
    std::array<T, 3> e;

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
        return vec3<T>::e[0];
    }

    auto y() const -> T
    {
        return vec3<T>::e[1];
    }

    auto z() const -> T
    {
        return vec3<T>::e[2];
    }

    auto x() -> T &
    {
        return vec3<T>::e[0];
    }

    auto y() -> T &
    {
        return vec3<T>::e[1];
    }

    auto z() -> T &
    {
        return vec3<T>::e[2];
    }

    auto operator[](int i) const -> T
    {
        return e[i];
    }

    auto operator[](int i) -> T & 
    {
        return e[i];
    }

    auto operator-() const -> vec3<T>
    { 
        return {-e[0], -e[1], -e[2]}; 
    }

    auto operator+=(const vec3<T> v) -> vec3<T> & 
    {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }

    auto operator-=(const vec3<T> v) -> vec3<T> & 
    {
        *this += -v;
        return *this;
    }

    auto operator*=(const vec3<T> v) -> vec3<T> &
    {
        e[0] *= v[0];
        e[1] *= v[1];
        e[2] *= v[2];
        return *this;
    }

    template<typename U>
    auto operator*=(const U u) -> vec3<T> &
    {
        e[0] *= static_cast<T>(u);
        e[1] *= static_cast<T>(u);
        e[2] *= static_cast<T>(u);
        return *this;
    }

    template<typename U>
    auto operator/=(const U u) -> vec3<T> & 
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

    auto unit_vector() const -> vec3<T> 
    {
        return *this / length();
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

template <typename T, typename U>
inline auto operator*(const U u, vec3<T> v) -> vec3<T> 
{
    v *= u;
    return v;
}

template <typename T, typename U>
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
class coord : public vec3<T>
{
public:
    coord() = default;
    coord(std::initializer_list<T> l) : vec3<T>{l} {}
    explicit coord(vec3<T> v) : vec3<T>{std::move(v)} {};
};