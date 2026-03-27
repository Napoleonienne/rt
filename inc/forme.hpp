#pragma once
#include "ray.h"
#include "hit.hpp"



namespace object{
class sphere : public hittable {
  public:
    sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        auto a = glm::dot(r.direction(), r.direction());
        auto h = dot(r.direction(), oc);
        auto c = glm::dot(oc, oc) - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - center) / radius;
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

  private:
    point3 center;
    double radius;
};


class box : public hittable{
public:
      box(const point3& center, double hauteur,double largeur) : center(center), hauteur(hauteur), largeur(largeur) {}





      bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        vec3 a =center - vec3(largeur/2,hauteur/2,0);
        vec3 b =center + vec3(largeur/2,hauteur/2,0);
        vec3 c = center + vec3(largeur/2,-hauteur/2,0);
        vec3 d = center - vec3(largeur/2,-hauteur/2,0);

        return (object::sphere(a,0.01).hit(r, ray_tmin, ray_tmax, rec) || object::sphere(b,0.01).hit(r, ray_tmin, ray_tmax, rec) || object::sphere(c,0.01).hit(r, ray_tmin, ray_tmax, rec) || object::sphere(d,0.01).hit(r, ray_tmin, ray_tmax, rec));
        

      }
private:
    point3 center;
    double hauteur;
    double largeur;




};


}

