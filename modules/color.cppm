export module color;

import std;
import vector;

// color is an alias for vec3 (RGB components stored as doubles in [0, 1])
export using color = vec3;

// Write a single pixel's colour as a PPM line into the output stream
export void write_color(const color& pixel_color, std::ofstream& f) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate [0,1] component values to the byte range [0,255]
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    std::println(f, "{} {} {}", rbyte, gbyte, bbyte);
}
