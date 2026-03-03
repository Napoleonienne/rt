#include <vector.hpp>
#include <vector>






struct bufferm
{
    std::vector<vec3> buff;
    int largeur;
    int hauteur;
    bufferm(int largeur,int hauteur){
        this->hauteur = hauteur;
        this->largeur = largeur;
        this->buff = std::vector<vec3>(largeur*hauteur);
    }
    vec3 operator[](int x,int y){
        int pos = (y*this->largeur) +x;
        return this->buff.at(pos);
    }




};