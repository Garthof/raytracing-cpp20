#pragma once

#include "vec3.hpp"

template<typename T>
class ray
{
public:
    ray() = default;

    ray(const coord<T> origin, const vec3<T> direction): orig{origin}, dir{direction}
    {}

    coord<T> origin() const  
    { 
        return orig; 
    }

    vec3<T> direction() const 
    { 
        return dir; 
    }

    coord<T> at(T t) const
    {
        return static_cast<coord<T>>(orig + t * dir);
    }

private:
    coord<T> orig;
    vec3<T> dir;
};