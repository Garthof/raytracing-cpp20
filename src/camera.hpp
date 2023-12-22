#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "random.hpp"
#include "vec3.hpp"

template <typename T>
class camera
{
public:
    double aspect_ratio = 1.;   // Ratio of image width over height
    int image_width = 100;      // Rendered image width in pixel count
    int samples_per_pixel = 10; // Count of random samples for each pixel
    int max_depth = 10;         // Maximum number of ray bounces into scene

    auto render(const hittable<T> &world) -> void 
    {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << m_image_height << "\n255\n";    

        for (auto j = 0; j < m_image_height; ++j) {        
            std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' ' << std::flush;
            
            for (auto i = 0; i < image_width; ++i) {
                auto pixel_color = color{0., 0., 0.};

                for (auto sample = 0; sample < samples_per_pixel; ++sample) {
                    const auto r = get_ray(i, j);
                    pixel_color += ray_color(std::move(r), max_depth, world);
                }
                std::cout << pixel{pixel_color, samples_per_pixel};
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int m_image_height{1};      // Rendered image height
    coord<T> m_center{};        // Camera center
    coord<T> m_pixel00_loc{};   // Location of pixel 0, 0
    vec3<T> m_pixel_delta_u{};  // Offset to pixel to the right
    vec3<T> m_pixel_delta_v{};  // Offset to pixel below
    
    auto initialize() -> void 
    {
        // Image height must be at least 1
        m_image_height = [&] {
            const auto tentative_image_height = static_cast<int>(image_width / aspect_ratio);
            return tentative_image_height < 1 ? 1 : tentative_image_height;
        }();

        m_center = coord{0., 0., 0.};

        // Determine viewport dimensions
        constexpr auto focal_length = 1.0;
        constexpr auto viewport_height = 2.0;
        const auto viewport_width = viewport_height * (static_cast<double>(image_width) / m_image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        const auto viewport_u = vec3{viewport_width, 0., 0.};
        const auto viewport_v = vec3{0., -viewport_height, -0.};

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        m_pixel_delta_u = viewport_u / image_width;
        m_pixel_delta_v = viewport_v / m_image_height;

        // Calculate the location of the upper left pixel.
        const auto viewport_upper_left = m_center - vec3{0., 0., focal_length} - viewport_u / 2 - viewport_v / 2;
        m_pixel00_loc = static_cast<coord<T>>(viewport_upper_left + (m_pixel_delta_u + m_pixel_delta_v) / 2.);
    }

    auto get_ray(const int i, const int j) -> ray<T>
    {
        // Get a randomly sampled camera ray for the pixel at location i,j
        const auto pixel_center = m_pixel00_loc + (i * m_pixel_delta_u) + (j * m_pixel_delta_v);
        const auto pixel_sample = pixel_center + pixel_sample_square();

        const auto ray_direction = pixel_sample - m_center;
        return {m_center, ray_direction};
    }

    auto pixel_sample_square() const -> vec3<T>
    {
        // Compute a random point in the square surrounding a pixel at the origin
        const auto px = -0.5 + rt::random_t<T>();
        const auto py = -0.5 + rt::random_t<T>();
        return (px * m_pixel_delta_u) + (py * m_pixel_delta_v);
    }

    auto ray_color(const ray<T> r, const int depth, const hittable<T> &world) const -> color<T> 
    {
        // If we've exceeded the ray bounce limit, no more light is gathered
        if (depth <= 0) {
            return {};
        }

        const auto unit_direction = r.direction.unit_vector();
        if (const auto rec = world.hit(std::move(r), {0., rt::infinity})) {
            const auto direction = rt::random_unit_vec_on_hemisphere(rec->normal);
            return static_cast<color<T>>(0.5 * ray_color({rec->pos, direction}, depth - 1, world));
        }

        const auto a = (unit_direction.y() + 1.0) * 0.5;
        return static_cast<color<T>>((1.0 - a) * color{1.0, 1.0, 1.0} + a * color{0.5, 0.7, 1.0});
    } 
};