#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

class Sphere : public Hitable {
public:
  Sphere() {};
  Sphere(vec3 c, float r, Material* mat) : center(c), radius(r), material(mat) {};
  virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord* out_rec) const;
  vec3 center;
  float radius;
  Material* material;
};

bool Sphere::Hit(const Ray& r, float t_min, float t_max, HitRecord* out_rec) const {
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b*b - a*c;
  if (discriminant > 0) {
    float t = (-b - sqrt(discriminant))/a;
    if (t_min < t && t < t_max) {
      out_rec->t = t;
      out_rec->hit_point = r.point_at_parameter(out_rec->t);
      out_rec->normal = (out_rec->hit_point - center) / radius; 
      out_rec->material_ptr = material;
      return true;
    }
    t = (-b + sqrt(discriminant))/a;
    if (t_min < t && t < t_max) {
      out_rec->t = t;
      out_rec->hit_point = r.point_at_parameter(out_rec->t);
      out_rec->normal = (out_rec->hit_point - center) / radius;
      out_rec->material_ptr = material;
      return true;
    }
  }
  return false;
}
#endif
