#pragma once

#include <initializer_list>
#include <memory>
#include <utility>
#include <vector>

#include "hittable.hpp"

template<typename T>
class hittable_list : public hittable<T>
{
public:
    using object_type = std::shared_ptr<hittable<T>>;
    std::vector<object_type> objects;

    hittable_list() = default;

    explicit hittable_list(const std::initializer_list<object_type> list) 
    {
        for (const auto obj: list) {
            add(std::move(obj));
        }
    }

    auto add(object_type obj) -> void 
    {
        objects.emplace_back(std::move(obj));
    }

    auto hit(const ray<T> r, const T ray_tmin, const T ray_tmax) const -> std::optional<hit_record<T>> override
    {
        auto recs = std::vector<hit_record<T>>{};
        auto closest_so_far = ray_tmax;

        for (const auto &obj : objects) {
            if (const auto temp_rec = obj->hit(r, ray_tmin, closest_so_far)) {
                recs.emplace_back(*temp_rec);
                closest_so_far = temp_rec->t;
            }
        }

        return recs.empty()? std::nullopt : std::optional<hit_record<T>>(recs.back());
    }

};