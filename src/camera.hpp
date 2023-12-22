#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "vec3.hpp"

template <typename T>
class camera
{
public:
    double aspect_ratio = 1.;   // Ratio of image width over height
    int image_width = 100;      // Rendered image width in pixel count

    auto render(const hittable<T> &world) -> void 
    {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << m_image_height << "\n255\n";    

        for (auto j = 0; j < m_image_height; ++j) {        
            std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' ' << std::flush;
            
            for (auto i = 0; i < image_width; ++i) {
                const auto pixel_center = m_pixel00_loc + (i * m_pixel_delta_u) + (j * m_pixel_delta_v);
                const auto ray_direction = pixel_center - m_center;
                const auto r = ray{m_center, ray_direction};

                const auto pixel_color = ray_color(r, world);
                std::cout << pixel_color;
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

    auto ray_color(const ray<T> r, const hittable<T> &world) const -> color<T> 
    {
        const auto unit_direction = r.direction.unit_vector();
        if (const auto rec = world.hit(std::move(r), {0., rt::infinity})) {
            return static_cast<color<T>>(0.5 * (rec->normal + vec3{1.0, 1.0, 1.0}));
        }

        const auto a = (unit_direction.y() + 1.0) * 0.5;
        return static_cast<color<T>>((1.0 - a) * color{1.0, 1.0, 1.0} + a * color{0.5, 0.7, 1.0});
    } 
};