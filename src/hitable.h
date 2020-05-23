#ifndef HITABLE_H
#define HITABLE_H
#include "ray.h"

class Material;

struct HitRecord {
  float t;
  vec3 hit_point;
  vec3 normal;
  Material* material_ptr;
};

class Hitable {
public:
  virtual bool Hit (const Ray& ray, float t_min, float t_max, HitRecord* out_rec) const = 0;
};
#endif
