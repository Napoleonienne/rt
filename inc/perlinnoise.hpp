#pragma once
#include <cmath>
#include "vector.hpp"
#include <utility.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <array>







//template<typename T>
class perlin{


public:
    perlin(const int seed,int Octaves,float Persistance,float Lacunarite=2.):seed(seed), Octaves(Octaves), Lacunarite(Lacunarite), Persistance(Persistance){
      remplir_tab();
      
    }


    bufferm<int> image(const int l, const int h){

      bufferm<int> bil(l,h);
      auto pixels = std::views::cartesian_product(
        std::views::iota(0, l), // Les lignes (j)
        std::views::iota(0, h)   // Les colonnes (i)
    );

    for (auto [j, i] : pixels) {

      bil[j,i] = std::floor(valeur_pixel2d(vec2(i,j))*255.0);


    }
    return bil;

  }







    double valeur_pixel2d(vec2   point){
      double va = 0.;
      double max=0.;
      double fr = 1.0;
      double per = 1.0;


      for(auto _ :std::views::iota(Octaves)){
        
        va += valeur_pixel_iter12d(point*fr)*per;
        max +=per;
        per *= Persistance;
        fr *= Lacunarite;

      }
      return ((va/max)+1)/2.0;

    }


  








    
    private:

    int Octaves;
    const int seed;
    float Lacunarite;
    float Persistance;
    std::array<double, 512> tableau;





    double valeur_pixel_iter12d(vec2 point){
      double x = std::floor(point.x);
      double y = std::floor(point.y);

      vec2 c1(x,y);
      vec2 c2(x+1,y);
      vec2 c3(x,y+1);
      vec2 c4(x+1,y+1);


      auto v1 = vecteur_aleatoire(c1);
      auto v2 = vecteur_aleatoire(c2);
      auto v3 = vecteur_aleatoire(c3);
      auto v4 = vecteur_aleatoire(c4);


      std::array<float,4> scalaire;
      
          vec2 nv = point -  c1;
          scalaire[0] =dot(v1, nv);
          nv = point -  c2;
          scalaire[1] =dot(v2, nv);
          nv = point -  c3;
          scalaire[2] =dot(v3, nv);
          nv = point -  c4;
          scalaire[3] =dot(v4, nv); 





      float sx = point.x - x;
      float sy = point.y - y;
      
      
      float a = interpolate(scalaire[0], scalaire[1], sx);
      float b = interpolate(scalaire[2], scalaire[3], sx);
      return interpolate(a, b, sy);

    


    }


    void remplir_tab(){
  std::iota(tableau.begin(), tableau.begin() + 256, 0);     
  std::mt19937 moteur_entropique(seed);
    std::shuffle(tableau.begin(), tableau.begin() + 256, moteur_entropique);
    std::copy(tableau.begin(), tableau.begin() + 256, tableau.begin() + 256);

    }


  float clamp(float x,const float lowerlimit = 0.0f,const float upperlimit = 1.0f) {
  if (x < lowerlimit) return lowerlimit;
  if (x > upperlimit) return upperlimit;
  return x;
  }

  float smootherstep(float x) {
  // Scale, and clamp x to 0..1 range
  x = clamp(x);

  return x * x * x * (x * (6.0f * x - 15.0f) + 10.0f);
  }
       float interpolate(float a0, float a1, float w) {
     return a0 + (a1 - a0) * smootherstep(w);
 }

  constexpr double hash2d(const vec2 input){
    int X = int(std::floor(input.x)) & 255;
    int Y = int(std::floor(input.y)) & 255;
    double res = tableau[tableau[X] + Y];

    return res;
  }


  constexpr double hash3d(const vec3 input){
    int X = int(std::floor(input.x)) & 255;
    int Y = int(std::floor(input.y)) & 255;
    int Z = int(std::floor(input.z)) & 255;
    double res = tableau[tableau[tableau[X] + Y]];

    return res;
  }


  inline vec2 vecteur_aleatoire(const vec2 v){
  double angle = (hash2d(v)/255.0)*glm::pi<double>()*2;
  vec2 res = {glm::cos(angle),glm::sin(angle)};


  return res;

  }







};

