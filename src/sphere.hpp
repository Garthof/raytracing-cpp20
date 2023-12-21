#pragma once

#include <cmath>

#include "hittable.hpp"

template <typename T>
class sphere : public hittable<T>
{
public:
    sphere(coord<T> t_center, T t_radius) : center{std::move(t_center)}, radius{t_radius}
    {}

    auto hit(ray<T> r, interval<T> ray_t) const -> std::optional<hit_record<T>> override 
    {
        const auto sqr = [](const auto v) {
            return v * v;
        };

        const auto oc = r.origin - center;
        const auto a = r.direction.length_squared();
        const auto half_b = dot(oc, r.direction);
        const auto c = oc.length_squared() - sqr(radius);
        
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
            const auto outward_normal = static_cast<vec3<T>>((pos - center) / radius).unit_vector();
            return hit_record<T>{std::move(r), *root, outward_normal};
        }

        return std::nullopt;
    }

    const coord<T> center;
    const T radius;
};