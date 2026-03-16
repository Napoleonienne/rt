#pragma once
#include <cmath>
#include "vector.hpp"
#include <utility.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <array>
#include <execution>


 std::array<vec2, 32> gradients_32 = {{
    { 1.000,  0.000}, { 0.981,  0.195}, { 0.924,  0.383}, { 0.831,  0.556},
    { 0.707,  0.707}, { 0.556,  0.831}, { 0.383,  0.924}, { 0.195,  0.981},
    { 0.000,  1.000}, {-0.195,  0.981}, {-0.383,  0.924}, {-0.556,  0.831},
    {-0.707,  0.707}, {-0.831,  0.556}, {-0.924,  0.383}, {-0.981,  0.195},
    {-1.000,  0.000}, {-0.981, -0.195}, {-0.924, -0.383}, {-0.831, -0.556},
    {-0.707, -0.707}, {-0.556, -0.831}, {-0.383, -0.924}, {-0.195, -0.981},
    {-0.000, -1.000}, { 0.195, -0.981}, { 0.383, -0.924}, { 0.556, -0.831},
    { 0.707, -0.707}, { 0.831, -0.556}, { 0.924, -0.383}, { 0.981, -0.195}
}};


namespace noise{
//template<typename T>
class perlin{


public:
    perlin(const int seed,int Octaves,float Persistance,float Lacunarite=2.):seed(seed), Octaves(Octaves), Lacunarite(Lacunarite), Persistance(Persistance){
      remplir_tab();
      
    }


    [[nodiscard]]  bufferm<int> image(const int l, const int h){
      

      bufferm<int> bil(l,h);
      auto pixels = std::views::cartesian_product(
        std::views::iota(0, l), // Les lignes (j)
        std::views::iota(0, h)   // Les colonnes (i)
    );

    
std::for_each(std::execution::par, pixels.begin(), pixels.end(), [&](auto coord) {
    auto [j, i] = coord;
    bil[j, i] = std::floor(valeur_pixel2d(vec2(i, j)) * 255.0);
});
    return bil;

  }



    double valeur_pixel2d(vec2   point){
      point = point / 32.0;
      double va = 0.;
      double max=0.;
      double fr = 1.0;
      double per = 1.0;


      for(auto _ :std::views::iota(0,Octaves)){
        
        va += valeur_pixel_iter12d(point*fr)*per;
        max +=per;
        per *= Persistance;
        fr *= Lacunarite;

      }
      return ((va/max)+1)/2.0;

    }


  








    
    private:
    double max1 = 1.0*std::pow(Persistance,Octaves-1);
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

  constexpr int hash2d(const vec2 input){
    int X = int(std::floor(input.x)) & 255;
    int Y = int(std::floor(input.y)) & 255;
    int res = tableau[tableau[X] + Y];

    return res;
  }





  inline vec2 vecteur_aleatoire(const vec2 v){
  double va = hash2d(v)&31;
  return gradients_32[va];

  }
 




};



class opensimplex{


public:
    opensimplex(const int seed,const int Octaves,const float Persistance,const float Lacunarite=2.):seed(seed), Octaves(Octaves), Lacunarite(Lacunarite), Persistance(Persistance){
      remplir_tab();
      
    }


    [[nodiscard]]  bufferm<int> image(const int l, const int h){
      

      bufferm<int> bil(l,h);
      auto pixels = std::views::cartesian_product(
        std::views::iota(0, l), // Les lignes (j)
        std::views::iota(0, h)   // Les colonnes (i)
    );

    
std::for_each(std::execution::par, pixels.begin(), pixels.end(), [&](auto coord) {
    auto [j, i] = coord;
    bil[j, i] = std::floor(valeur_pixel2d(vec2(i, j)/resolution) * 255.0);
});
    return bil;

  }

  void set_resolution(float va){ this->resolution = va;}


    double valeur_pixel2d(vec2   point){
      point = point / 32.0;
      double va = 0.;
      double max=0.;
      double fr = 1.0;
      double per = 1.0;
      float resolution=32.F;


      for(auto _ :std::views::iota(0,Octaves)){
        
        va += valeur_pixel_iter12d(point*fr)*per;
        max +=per;
        per *= Persistance;
        fr *= Lacunarite;

      }
      return ((va/max)+1)/2.0;

    }
  
    private:
    double max1 = 1.0*std::pow(Persistance,Octaves-1);
    int Octaves;
    const int seed;
    float Lacunarite;
    float Persistance;
    std::array<double, 512> tableau;
    double G2 = (3.0 - std::sqrt(3.0)) / 6.0;
    float F2=(glm::sqrt(3.0)-1.)/2.;


    double valeur_pixel_iter12d(vec2 point){

      float bil = F2*(point.x+point.y);
    

      double x = std::floor(point.x+bil);
      double y = std::floor(point.y+bil);
      double t = (x + y) * G2;
      double X0 = x - t; 
      double Y0 = y - t;

      double dx0 = point.x - X0;
      double dy0 = point.y - Y0;


      vec2 p1= vec2(x,y);
      vec2 p2 = x>y ? vec2(x+1,y) :  vec2(x,y+1) ;
      vec2 p3 = vec2(x+1,y+1);





      auto v1 = vecteur_aleatoire(p1);
      auto v2 = vecteur_aleatoire(p2);
      auto v3 = vecteur_aleatoire(p3);

      double t0 = 0.5 - dx0*dx0 - dy0*dy0;
      if (t0 > 0) {
        t0 *= t0;
        // t0^4 * produit_scalaire
        contribution += (t0 * t0) * dot(vecteur_aleatoire(vec2(i, j)), vec2(dx0, dy0));
      }




      


    }
    


    void remplir_tab(){
  std::iota(tableau.begin(), tableau.begin() + 256, 0);     
  std::mt19937 moteur_entropique(seed);
    std::shuffle(tableau.begin(), tableau.begin() + 256, moteur_entropique);
    std::copy(tableau.begin(), tableau.begin() + 256, tableau.begin() + 256);

    }



  constexpr int hash2d(const vec2 input){
    int X = int(std::floor(input.x)) & 255;
    int Y = int(std::floor(input.y)) & 255;
    double res = tableau[tableau[X] + Y];

    return res;
  }





  inline vec2 vecteur_aleatoire(const vec2 v){
  int va = hash2d(v) &31;

  return gradients_32[va];

  }







};
}
