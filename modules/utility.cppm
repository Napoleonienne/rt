export module utility;

import std;
import vector;

// CPU-side pixel buffer — stores one vec3 per pixel, indexed by (x, y)
export struct bufferm {
    std::vector<vec3> buff;
    int largeur;
    int hauteur;

    bufferm(int l, int h) : buff(l * h), largeur(l), hauteur(h) {}

    vec3& operator[](int x, int y) {
        return buff[(y * largeur) + x];
    }
};
