#include <iostream>
#include <memory>

#include "camera.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "rtweekend.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

auto main() -> int
{

    // World
    hittable_list<rt::scalar_type> world;

    const auto ground_material = std::make_shared<lambertian<rt::scalar_type>>(color{0.5, 0.5, 0.5});
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{0., -1000., 0.}, 1000., ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            const auto choose_mat = rt::random_t<rt::scalar_type>();
            const auto center = coord{a + 0.9 * rt::random_t<rt::scalar_type>(), 0.2, b + 0.9 * rt::random_t<rt::scalar_type>()};

            if ((center - coord{4., 0.2, 0.}).length() > 0.9) {
                std::shared_ptr<material<rt::scalar_type>> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    const auto albedo = static_cast<color<rt::scalar_type>>(rt::random_v<rt::scalar_type>() * rt::random_v<rt::scalar_type>());
                    sphere_material = std::make_shared<lambertian<rt::scalar_type>>(albedo);
                    world.add(std::make_shared<sphere<rt::scalar_type>>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    const auto albedo = static_cast<color<rt::scalar_type>>(rt::random_v(0.5, 1.));
                    const auto fuzz = rt::random_t(0., 0.5);
                    sphere_material = std::make_shared<metal<rt::scalar_type>>(albedo, fuzz);
                    world.add(std::make_shared<sphere<rt::scalar_type>>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric<rt::scalar_type>>(1.5);
                    world.add(std::make_shared<sphere<rt::scalar_type>>(center, 0.2, sphere_material));
                }
            }
        }
    }

    const auto material1 = std::make_shared<dielectric<rt::scalar_type>>(1.5);
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{0., 1., 0.}, 1.0, material1));

    auto material2 = std::make_shared<lambertian<rt::scalar_type>>(color{0.4, 0.2, 0.1});
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{-4., 1., 0.}, 1.0, material2));

    auto material3 = std::make_shared<metal<rt::scalar_type>>(color{0.7, 0.6, 0.5}, 0.0);
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{4., 1., 0.}, 1.0, material3));

    // Camera

    camera<rt::scalar_type> cam;

    cam.aspect_ratio = 16. / 9.;
    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;
    
    cam.vfov = 20.;
    cam.lookfrom = coord{13.,  2.,  3.};
    cam.lookat   = coord{ 0.,  0.,  0.};
    cam.vup      = vec3 { 0. , 1.,  0.};

    cam.defocus_angle   = .6;
    cam.focus_dist      = 10.;

    // Render

    cam.render(world);
}