#include <iostream>
#include <memory>

#include "camera.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "rtweekend.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

auto main() -> int
{
    // World
    
    const auto material_ground = std::make_shared<lambertian<rt::scalar_type>>(color{0.8, 0.8, 0.0});
    // const auto material_center = std::make_shared<lambertian<rt::scalar_type>>(color{0.7, 0.3, 0.3});
    // const auto material_left   = std::make_shared<metal<rt::scalar_type>>(color{0.8, 0.8, 0.8}, 0.3);
    const auto material_center = std::make_shared<dielectric<rt::scalar_type>>(1.5);
    const auto material_left   = std::make_shared<dielectric<rt::scalar_type>>(1.5);
    const auto material_right  = std::make_shared<metal<rt::scalar_type>>(color{0.8, 0.6, 0.2}, 1.0);

    hittable_list<rt::scalar_type> world;
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{ 0.0, -100.5, -1.0}, 100.0, material_ground));
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{ 0.0,    0.0, -1.0},   0.5, material_center));
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{-1.0,    0.0, -1.0},   0.5, material_left));
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{ 1.0,    0.0, -1.0},   0.5, material_right));

    // Camera

    camera<rt::scalar_type> cam;
    cam.aspect_ratio = 16. / 9.;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    // Render

    cam.render(world);
}