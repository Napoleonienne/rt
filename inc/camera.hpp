#include <glm/glm.hpp>



class camera{

    public:
    camera(int h,int l){
        hauteur = h;
        largeur =l;
        viewport_height= 2.;
        viewport_width = viewport_height * (std::floor(largeur)/hauteur);
        viewport_u = glm::vec3(viewport_width, 0., 0.);
        viewport_v= glm::vec3(0, -viewport_height, 0.);
        pixel_delta_u = viewport_u / static_cast<float>(largeur);
        pixel_delta_v = viewport_v / static_cast<float>(hauteur);
        viewport_upper_left = camera_center - glm::vec3(0, 0, focal_length) - viewport_u/2.0f - viewport_v/2.0f  ;
        pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

        

    }

    
    private:
    int hauteur,largeur;
    float focal_length = 1.0;
    float viewport_height;
    float viewport_width;
    glm::vec3 camera_center = glm::vec3(0., 0., 0.);
    glm::vec3 viewport_u;
    glm::vec3 viewport_v;

    // Calculate the vectors across the horizontal and down the vertical viewport edges.

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    glm::vec3 pixel_delta_u ;
    glm::vec3 pixel_delta_v ;

    // Calculate the location of the upper left pixel.
    glm::vec3 viewport_upper_left;
    glm::vec3 pixel00_loc ;

};




