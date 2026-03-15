#include "vector.hpp"
#include "color.h"
#include "ray.h"
#include "utility.hpp"
#include "fstream"
#include <algorithm>
#include <ranges>
#include <perlinnoise.hpp>


constexpr auto aspect_ratio = 16.0 / 9.0;
constexpr int image_width = 800;

// Calculate the image height, and ensure that it's at least 1.
constexpr int image_height = int(image_width / aspect_ratio)< 1 ? 1 : int(image_width / aspect_ratio);

// Camera

auto focal_length = 1.0;
auto viewport_height = 2.0;
auto viewport_width = viewport_height * (double(image_width)/image_height);
auto camera_center = point3(0, 0, 0);

// Calculate the vectors across the horizontal and down the vertical viewport edges.
auto viewport_u = vec3(viewport_width, 0, 0);
auto viewport_v = vec3(0, -viewport_height, 0);

// Calculate the horizontal and vertical delta vectors from pixel to pixel.
auto pixel_delta_u = viewport_u / static_cast<double>(image_width);
auto pixel_delta_v = viewport_v / static_cast<double>(image_height);

// Calculate the location of the upper left pixel.
auto viewport_upper_left = camera_center
                            - vec3(0, 0, focal_length) - viewport_u/2.0 - viewport_v/2.0;
auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);









double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin();
    auto a = glm::dot(r.direction(), r.direction());
    auto h = dot(r.direction(), oc);
    auto c = glm::dot(oc, oc) - radius*radius;
    auto discriminant = h*h - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (h - std::sqrt(discriminant)) / a;
    }


}






color ray_color(const ray& r) {
    double hit = hit_sphere(point3(0,0,-1.f), 0.5, r);
    if (hit>0){

        vec3 N = glm::normalize(r.at(hit) - vec3(0,0,-1));
        return 0.5*color(N.x+1, N.y+1, N.z+1);
    }

    vec3 unit_direction = glm::normalize(r.direction());
    auto a = 0.5*(unit_direction.y + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}


void dump(const bufferm<vec3> &buffer, std::ofstream &f) {
    std::ranges::for_each(buffer.buff, [&f](const vec3 &vec) {
        write_color(vec, f);
    });
}






void render(bufferm<vec3>& buffer){
// Dans la fonction vide
    auto pixels = std::views::cartesian_product(
        std::views::iota(0, image_height), // Les lignes (j)
        std::views::iota(0, image_width)   // Les colonnes (i)
    );

    for (auto [j, i] : pixels) {
        auto pixel_center = pixel00_loc + (static_cast<double>(i) * pixel_delta_u) + (static_cast<double>(j) * pixel_delta_v);
        auto ray_direction = pixel_center - camera_center;
        ray r(camera_center, ray_direction);

        buffer[i, j] = ray_color(r);
    }


}








int main(){


// Dans le main
    bufferm<vec3> buffer(image_width,image_height);
    // Render
    for(int k: std::views::iota(0, 1) ) {
        auto op = std::format("image{}.ppm",k);
        std::ofstream FILE(op) ;
        std::println(FILE, "P3\n{} {} \n255", image_width, image_height);
        render(buffer);





        // std::clog << "\rframes : " << k << ' ' << std::flush;




    dump(buffer,FILE);
    //std::clog << "\rDone.                 \n";


}
    perlin test(1414,8,0.5);

std::ofstream file("perlin.pgm") ;
std::println(file, "P2\n{} {} \n255", 512, 512);

for(int i: std::views::iota(0, 128)){
    for(int j: std::views::iota(0, 128)){
        std::println("{}",test.valeur_pixel2d(vec2(i,j))*255.0);
    }
}



return 0;
}