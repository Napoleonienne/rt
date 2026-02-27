#include <iostream>
#include <print>
#include <math.h>
#include <format>
#include "vector.hpp"
#include "color.h"
 #include "ray.h"




int main(){
    // Image

    int image_width = 400;
    int image_height = 800;

    // Render
    point3 origin(0, 0, 0);

    //determeiner si un object incept avec un rayon sereez juste de metre un t grand et voie si sa rentre dans une surface
    


    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.0;

     
            vec3 pixel_color(r, g, b);

            write_color( pixel_color);
        }
  
}

return 0;
}