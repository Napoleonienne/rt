#pragma once
#include <vector.hpp>
#include <vector>






template<typename t>
struct bufferm
{
    std::vector<t> buff;
    const int largeur;
    const int hauteur;
    bufferm(int l, int h) : buff(l * h), largeur(l), hauteur(h) {
    }

    t& operator[](int x, int y)  {
        return buff[(y * largeur) + x];
    }




};