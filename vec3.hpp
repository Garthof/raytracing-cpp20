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

    T x() const
    {
        return vec3<T>::e[0];
    }

    T y() const
    {
        return vec3<T>::e[1];
    }

    T z() const 
    {
        return vec3<T>::e[2];
    }

    T &x()
    {
        return vec3<T>::e[0];
    }

    T &y()
    {
        return vec3<T>::e[1];
    }

    T &z()
    {
        return vec3<T>::e[2];
    }

    T operator[](int i) const 
    {
        return e[i];
    }

    T& operator[](int i) 
    {
        return e[i];
    }

    vec3<T> operator-() const
    { 
        return {-e[0], -e[1], -e[2]}; 
    }

    vec3<T>& operator+=(const vec3<T> v)
    {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }

    vec3<T>& operator-=(const vec3<T> v)
    {
        *this += -v;
        return *this;
    }

    inline vec3<T> &operator*=(const vec3<T> v)
    {
        e[0] *= v[0];
        e[1] *= v[1];
        e[2] *= v[2];
        return *this;
    }

    template<typename U>
    vec3<T>& operator*=(const U u)
    {
        e[0] *= static_cast<T>(u);
        e[1] *= static_cast<T>(u);
        e[2] *= static_cast<T>(u);
        return *this;
    }

    template<typename U>
    vec3<T>& operator/=(const U u)
    {
        return *this *= static_cast<T>(1) / static_cast<T>(u);
    }

    T length() const 
    {
        return std::sqrt(length_squared());
    }

    T length_squared() const 
    {
        return std::accumulate(e.begin(), e.end(), T{}, 
                               [](const T a, const T b) { return a + b * b; });
    }

    vec3<T> unit_vector() const 
    {
        return *this / length();
    }
};

template <typename T>
inline std::ostream & operator<<(std::ostream &out, const vec3<T> v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

template <typename T>
inline vec3<T> operator+(vec3<T> lhs, const vec3<T> rhs) 
{
    lhs += rhs;
    return lhs;
}

template <typename T>
inline vec3<T> operator-(vec3<T> lhs, const vec3<T> rhs) 
{
    lhs -= rhs;
    return lhs;
}

template <typename T>
inline vec3<T> operator*(vec3<T> lhs, const vec3<T> rhs) 
{
    lhs *= rhs;
    return lhs;
}

template <typename T, typename U>
inline vec3<T> operator*(const U u, vec3<T> v) 
{
    v *= u;
    return v;
}

template <typename T, typename U>
inline vec3<T> operator*(vec3<T> v, const U u) 
{
    return u * std::move(v);
}

template <typename T, typename U>
inline vec3<T> operator/(vec3<T> v, const U u) 
{
    v /= u;
    return v;
}

template <typename T>
inline T dot(const vec3<T> lhs, const vec3<T> rhs) 
{
    const auto product = lhs * rhs;
    return std::accumulate(product.e.begin(), product.e.end(), T{});
}

template <typename T>
inline vec3<T> cross(const vec3<T> u, const vec3<T> v) {
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