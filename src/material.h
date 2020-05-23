#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include "hitable.h"
#include "ray.h"

class Material {
public:
  virtual bool Scatter(
    const Ray& ray_in, const HitRecord& hit_record, 
    vec3* out_attenuation, Ray* out_scattered) const = 0; 
};

class Lambertian : public Material {
public:
  Lambertian(const vec3& albedo) : albedo_(albedo) {}
  bool Scatter(
      const Ray& ray_in, const HitRecord& hit_record, 
      vec3* out_attenuation, Ray* out_scattered) const override {
    vec3 target = hit_record.hit_point + hit_record.normal + RandomInUnitSphere();
    *out_scattered = Ray(hit_record.hit_point, target - hit_record.hit_point);
    *out_attenuation = albedo_;
    return true;
  };
  vec3 albedo_;
};

class Metal : public Material {
public:
  Metal(const vec3& albedo) : albedo_(albedo) {}
  bool Scatter(
      const Ray& ray_in, const HitRecord& hit_record, 
      vec3* out_attenuation, Ray* out_scattered) const override {
    vec3 reflected = Reflect(unit_vector(ray_in.direction()), hit_record.normal);
    *out_scattered = Ray(hit_record.hit_point, reflected);
    *out_attenuation = albedo_;
    return (dot(out_scattered->direction(), hit_record.normal) > 0);
  }
  vec3 albedo_;
};
#endif // MATERIAL_H
