export module ray;

import std;
import vector;

// A ray: P(t) = origin + t * direction
export class ray {
public:
    ray() = default;
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    const point3& origin()    const { return orig; }
    const vec3&   direction() const { return dir;  }

    [[nodiscard]] constexpr point3 at(double t) const {
        return orig + t * dir;
    }

private:
    point3 orig;
    vec3   dir;
};

// Record of a ray–surface intersection
export class hit_record {
public:
    point3 p;
    vec3   normal;
    double t{};
    bool   front_face{};

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // outward_normal is assumed to have unit length
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// Abstract base for any hittable surface
export class hittable {
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

export namespace object {

class sphere : public hittable {
public:
    sphere(const point3& center, double radius)
        : center(center), radius(std::fmax(0, radius)) {}

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        auto a  = r.direction().length_squared();
        auto h  = dot(r.direction(), oc);
        auto c  = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root within the acceptable range
        auto root = (h - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        rec.t      = root;
        rec.p      = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

private:
    point3 center;
    double radius;
};

class box : public hittable {
public:
    box(const point3& center, double hauteur, double largeur)
        : center(center), hauteur(hauteur), largeur(largeur) {}

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        vec3 a = center - vec3(largeur / 2, hauteur / 2, 0);
        vec3 b = center + vec3(largeur / 2, hauteur / 2, 0);
        vec3 c = center + vec3(largeur / 2, -hauteur / 2, 0);
        vec3 d = center - vec3(largeur / 2, -hauteur / 2, 0);
        (void)a; (void)b; (void)c; (void)d;
        return false; // TODO: implement AABB hit test
    }

private:
    point3 center;
    double hauteur;
    double largeur;
};

} // namespace object

// A list of hittable objects — tests each in turn and returns the closest hit
export class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() = default;
    explicit hittable_list(std::shared_ptr<hittable> object) { add(std::move(object)); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<hittable> object) {
        objects.push_back(std::move(object));
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything  = false;
        auto closest_so_far = ray_tmax;

        for (const auto& object : objects) {
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything    = true;
                closest_so_far  = temp_rec.t;
                rec             = temp_rec;
            }
        }

        return hit_anything;
    }
};
