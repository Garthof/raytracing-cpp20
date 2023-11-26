#pragma once

#include <optional>

#include "ray.hpp"
#include "vec3.hpp"

template <typename T>
struct hit_record
{
    const T t;
    const coord<T> pos;
    const bool front_face;
    const vec3<T> normal;

    /// @param outward_normal Unit length vector at the hit position facing outwards
    hit_record(const ray<T> r, const T t, const vec3<T> outward_normal) : 
        t{t}, pos{r.at(t)}, front_face{dot(r.direction, outward_normal) < 0.0},
        normal{front_face ? outward_normal : -outward_normal} {};
};

template <typename T>
class hittable
{
public:
    virtual ~hittable() = default;
    virtual auto hit(ray<T> r, T ray_tmin, T ray_tmax) const -> std::optional<hit_record<T>> = 0;
};