#include <iostream>

#include "color.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "ray.hpp"
#include "rtweekend.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

template <typename T>
auto ray_color(const ray<T> r, const hittable<T> &world) -> color<T>
{
    const auto unit_direction = r.direction.unit_vector();
    if (const auto rec = world.hit(std::move(r), 0., rt::infinity)) {
        return static_cast<color<T>>(0.5 * (rec->normal + vec3{1.0, 1.0, 1.0}));
    }

    const auto a = (unit_direction.y() + 1.0) * 0.5;
    return static_cast<color<T>>((1.0 - a) * color{1.0, 1.0, 1.0} + a * color{0.5, 0.7, 1.0});
}

auto main() -> int
{
    // Image

    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr auto image_width = 400;

    // Image height must be at least 1
    constexpr auto  image_height = [&] {
        const auto tentative_image_height = static_cast<int>(image_width / aspect_ratio);
        return tentative_image_height < 1 ? 1 : tentative_image_height;
    }();

    // World

    hittable_list<rt::scalar_type> world;
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{0., 0., -1.}, 0.5));
    world.add(std::make_shared<sphere<rt::scalar_type>>(coord{0., -100.5, -1.}, 100.));

    // Camera

    constexpr auto focal_length = 1.0;
    constexpr auto viewport_height = 2.0;
    constexpr auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    const auto camera_center = coord{0., 0., 0.};

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    const auto viewport_u = vec3{viewport_width, 0., 0.};
    const auto viewport_v = vec3{0., -viewport_height, -0.};

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    const auto pixel_delta_u = viewport_u / image_width;
    const auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    const auto viewport_upper_left = camera_center - vec3{0., 0., focal_length} - viewport_u / 2 - viewport_v / 2;
    const auto pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) / 2;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";    

    for (auto j = 0; j < image_height; ++j) {        
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        
        for (auto i = 0; i < image_width; ++i) {
            const auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            const auto ray_direction = pixel_center - camera_center;
            const auto r = ray{camera_center, ray_direction};

            const auto pixel_color = ray_color(r, world);
            std::cout << pixel_color;
        }
    }

    std::clog << "\rDone.                 \n";
}