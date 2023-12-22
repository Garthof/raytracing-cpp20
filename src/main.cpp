#include <iostream>

#include "camera.hpp"
#include "color.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "ray.hpp"
#include "rtweekend.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

auto main() -> int
{
    // World

    hittable_list<rt::scalar_type> world;
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{0., 0., -1.}, 0.5));
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{0., -100.5, -1.}, 100.));

    // Camera

    camera<rt::scalar_type> cam;
    cam.aspect_ratio = 16. / 9.;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    // Render

    cam.render(world);
}