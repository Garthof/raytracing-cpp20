#pragma once

#include <cmath>

#include "hittable.hpp"

template <typename T>
class sphere : public hittable<T>
{
public:
    auto hit(ray<T> r, T ray_tmin, T ray_tmax) const -> std::optional<hit_record<T>> override 
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

        const auto in_range = [&](const auto v) {
            return ray_tmin < v && v < ray_tmax;
        };

        const auto find_root_in_range = [&]() -> std::optional<T> {
            auto root = (-half_b - sqrtd) / a;
            if (!in_range(root)) {
                root = (-half_b + sqrtd) / a;
            }
            return in_range(root) ? root : std::nullopt;
        };

        if (const auto root = find_root_in_range()) {
            const auto pos = r.at(root);
            const auto outward_normal = static_cast<vec3<T>>((pos - center) / radius).unit_length();
            return hit_record<T>{std::move(r), root, outward_normal};
        }

        return std::nullopt;
    }

    const coord<T> center;
    const T radius;
};