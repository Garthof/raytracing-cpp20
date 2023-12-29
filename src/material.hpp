#pragma once

#include <optional>

#include "color.hpp"
#include "random.hpp"
#include "ray.hpp"

template<typename T> struct hit_record;

template<typename T>
struct scatter_result
{
    ray<T> scattered;
    color<T> attenuation;
};

template<typename T>
class material
{
public:
    virtual ~material() = default;
    virtual auto scatter(ray<T> r_in, hit_record<T> rec) -> std::optional<scatter_result<T>> const = 0;
};

template<typename T>
class lambertian : public material<T>
{
public:
    lambertian(color<T> t_albedo) : m_albedo{std::move(t_albedo)} {}

    auto scatter(ray<T>, hit_record<T> rec) -> std::optional<scatter_result<T>> const override
    {
        auto scatter_direction = rec.normal + rt::random_unit_vec_on_sphere<T>();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }
        return scatter_result{{rec.pos, scatter_direction}, m_albedo};
    }

private:
    color<T> m_albedo;
};

template<typename T>
class metal: public material<T>
{
public:
    metal(color<T> t_albedo, T t_fuzz) 
        : m_albedo{std::move(t_albedo)}, m_fuzz{t_fuzz < 1. ? t_fuzz : 1.} {}

    auto scatter(ray<T> r_in, hit_record<T> rec) -> std::optional<scatter_result<T>> const override
    {
        const auto reflected = reflect(r_in.direction.unit_vector(), rec.normal);
        return scatter_result{{rec.pos, reflected + m_fuzz * rt::random_unit_vec_on_sphere<T>()}, 
                              m_albedo};
    }

private:
    color<T> m_albedo;
    T m_fuzz;
};

template<typename T>
class dielectric : public material<T>
{
public:
    dielectric(const T t_index_of_refraction): m_ir{t_index_of_refraction} {}

    auto scatter(ray<T> r_in, hit_record<T> rec) -> std::optional<scatter_result<T>> const override
    {
        const auto attenuation = color{1., 1., 1.};
        const auto refraction_ratio = rec.front_face ? 1. / m_ir : m_ir;

        const auto unit_direction = r_in.direction.unit_vector();
        const auto refracted = refract(unit_direction, rec.normal, refraction_ratio);

        return scatter_result{{rec.pos, refracted}, attenuation};
    }  

private:
    T m_ir; // Index of refraction
};