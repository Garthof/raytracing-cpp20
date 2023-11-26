#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "hittable.hpp"

template<typename T>
class hittable_list : public hittable<T>
{
public:
    using object_t = std::shared_ptr<hittable<const T>>;
    std::vector<object_t> objects;

    hittable_list() = default;

    explicit hittable_list(const std::initializer_list<const T> l) 
    {
        for (const auto &t: l) {
            add(t);
        }
    }

    auto add(const T &t) -> void 
    {
        objects.push_back(t);
    }

    auto hit(const ray<T> r, const T ray_tmin, const T ray_tmax) const -> std::optional<hit_record<T>> override
    {
        std::optional<hit_record<T>> rec = std::nullopt;

        auto hit_anything = false;
        auto closest_so_far = ray_tmax;

        for (const auto &obj : objects) {
            if (const auto temp_rec = obj->hit(r, ray_tmin, closest_so_far)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return rec;
    }

};