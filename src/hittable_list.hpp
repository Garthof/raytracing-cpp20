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

    auto hit(const ray<T> r, const interval<T> ray_t) const -> std::optional<hit_record<T>> override
    {
        auto closest_so_far = ray_t.max;
        auto rec = std::optional<hit_record<T>>{};

        for (const auto &obj : objects) {
            if (auto rec_found = obj->hit(r, {ray_t.min, closest_so_far})) {
                rec = std::move(rec_found);
                closest_so_far = rec->t;            
            }
        }

        return rec;
    }

};