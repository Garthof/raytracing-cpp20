#pragma once

#include <cmath>

#include "hittable.hpp"
#include "material.hpp"

template <typename T>
class sphere : public hittable<T>
{
public:
    using material_type = std::shared_ptr<material<T>>;

    sphere(coord<T> t_center, T t_radius, material_type t_material) 
        : m_center{std::move(t_center)}, m_radius{t_radius}, m_material{t_material}
    {}

    auto hit(ray<T> r, interval<T> ray_t) const -> std::optional<hit_record<T>> override 
    {
        const auto sqr = [](const auto v) {
            return v * v;
        };

        const auto oc = r.origin - m_center;
        const auto a = r.direction.length_squared();
        const auto half_b = dot(oc, r.direction);
        const auto c = oc.length_squared() - sqr(m_radius);
        
        const auto discriminant = sqr(half_b) - a*c;
        if (discriminant < 0) {
            return std::nullopt;
        }
        const auto sqrtd = std::sqrt(discriminant);

        const auto find_root_in_range = [&]() {
            auto root = (-half_b - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (-half_b + sqrtd) / a;
            }
            return ray_t.surrounds(root) ? std::optional<T>{root} : std::nullopt;
        };

        if (const auto root = find_root_in_range()) {
            const auto pos = r.at(*root);
            const auto outward_normal = static_cast<vec3<T>>((pos - m_center) / m_radius).unit_vector();
            return hit_record{std::move(r), *root, outward_normal, m_material};
        }

        return std::nullopt;
    }

    coord<T> m_center;
    T m_radius;
    material_type m_material;
};