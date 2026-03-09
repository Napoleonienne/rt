#pragma once
#include "vector.hpp"
#include <iostream>
#include <print>
#include "cstdio"
#include <fstream>

using color = vec3;

void write_color( const color& pixel_color,std::ofstream &f) {
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    std::println(f,"{} {} {}", rbyte, gbyte, bbyte);
}