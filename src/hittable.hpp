#pragma once

#include <exception>
#include <optional>
#include <memory>

#include "material.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"

template<typename T>
class material;

template <typename T>
struct hit_record
{
    using material_type = std::shared_ptr<material<T>>;

    T t;
    coord<T> pos;
    bool front_face;
    vec3<T> normal;
    material_type mat;

    /// @param t_outward_normal Unit length vector at the hit position facing outwards
    hit_record(const ray<T> t_r, const T t_t, const vec3<T> t_outward_normal, material_type t_mat) 
        : t{t_t}, pos{t_r.at(t_t)}, front_face{dot(t_r.direction, t_outward_normal) < 0.0},
          normal{front_face ? t_outward_normal : -t_outward_normal}, mat{t_mat} {
            if (!mat) {
                throw std::invalid_argument{"Invalid material"};
            }
        };
};

template <typename T>
class hittable
{
public:
    virtual ~hittable() = default;
    virtual auto hit(ray<T> t_r, interval<T> ray_t) const -> std::optional<hit_record<T>> = 0;
};