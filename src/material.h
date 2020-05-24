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
  // Fuzz is the radius of a sphere that contains possible random endpoints for 
  // the reflected ray. 
  Metal(const vec3& albedo, float fuzz) : albedo_(albedo) {
    if (fuzz < 0) {
      fuzz_ = 0;
    } else if (fuzz > 1) {
      fuzz_ = 1;
    } else {
      fuzz_ = fuzz;
    }
  }
  bool Scatter(
      const Ray& ray_in, const HitRecord& hit_record, 
      vec3* out_attenuation, Ray* out_scattered) const override {
    vec3 reflected = Reflect(unit_vector(ray_in.direction()), hit_record.normal);
    *out_scattered = Ray(hit_record.hit_point, reflected + fuzz_*RandomInUnitSphere());
    *out_attenuation = albedo_;
    return (dot(out_scattered->direction(), hit_record.normal) > 0);
  }
  vec3 albedo_;
  float fuzz_;
};

class Dielectric : public Material {
public:
  Dielectric(float ref_idx) : ref_idx_(ref_idx) {}
  // Returns the Specular Reflection Coefficient using the Shlicks approximation.
  // Real glass has reflectivity that varies with angle, e.g. at grazing incidence
  // surfaces will appear mirror like, despite poorer reflectivity at normal 
  // incidence.
  float Schlick(float cosine, float ref_idx) const {
    // Note: 
    // r(0) = r0 = ((n1 - n2)/(n1 + n2))^2
    // r(theta) = r0 + (1 - r0)(1 - cos(theta))^5 
    // r(90) = r0 + (1 - r0) = 1, highest probability of reflection. 
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0)*pow((1 - cosine), 5);
  }

  virtual bool Scatter(
      const Ray& ray_in, const HitRecord& hit_record, vec3* out_attenuation, Ray* out_scattered) const {
    vec3 outward_normal;
    vec3 reflected = Reflect(ray_in.direction(), hit_record.normal);
    float ni_over_nt;
    // Attenuation is always 1.0 (the glass surface absorbs nothing) 
    *out_attenuation = vec3(1.0, 1.0, 1.0);
    vec3 refracted;
    float reflect_prob;
    float cosine;
    if (dot(ray_in.direction(), hit_record.normal) > 0) {
      outward_normal = -hit_record.normal;
      ni_over_nt = ref_idx_;
      cosine = ref_idx_ * dot(ray_in.direction(), hit_record.normal) / ray_in.direction().length();
    } else {
      outward_normal = hit_record.normal;
      ni_over_nt = 1.0 / ref_idx_;
      cosine = -dot(ray_in.direction(), hit_record.normal) / ray_in.direction().length();
    }
    if (Refract(ray_in.direction(), outward_normal, ni_over_nt, &refracted)) {
      reflect_prob = Schlick(cosine, ref_idx_);
    } else {
      reflect_prob = 1.0;
    }
    if (drand48() < reflect_prob) {
      *out_scattered = Ray(hit_record.hit_point, reflected);
    } else {
      *out_scattered = Ray(hit_record.hit_point, refracted);
    }
    return true;
  }
  float ref_idx_;
};
#endif // MATERIAL_H
