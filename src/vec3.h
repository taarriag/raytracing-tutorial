#ifndef RAYTRACING_VEC3_H
#define RAYTRACING_VEC3_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
public:
  vec3() {}
  vec3(float e0, float e1, float e2) {
    e[0] = e0;
    e[1] = e1;
    e[2] = e2;
  }
  inline float x() const { return e[0]; }
  inline float y() const { return e[1]; }
  inline float z() const { return e[2]; }
  inline float r() const { return e[0]; }
  inline float g() const { return e[1]; }
  inline float b() const { return e[2]; }

  inline const vec3& operator+() const { return *this; }
  inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  inline float operator[](int i) const { return e[i]; }
  inline float& operator[](int i) { return e[i]; }

  inline vec3& operator+=(const vec3& v2);
  inline vec3& operator-=(const vec3& v2);
  inline vec3& operator*=(const vec3& v2);
  inline vec3& operator/=(const vec3& v2);
  inline vec3& operator*=(const float t);
  inline vec3& operator/=(const float t);

  inline float length() const {
    return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
  }
  inline float squared_length() {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }
  inline void make_unit_vector();

  float e[3];
};

inline std::istream& operator>>(std::istream& is, vec3& v) {
  is >> v.e[0] >> v.e[1] >> v.e[2];
  return is;
}

inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
  os << v.e[0] << " " << v.e[1] << " " << v.e[2];
  return os; 
}

inline void vec3::make_unit_vector() {
  float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
  e[0] *= k;
  e[1] *= k;
  e[2] *= k;
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
  return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
  return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3& v1, const vec3& v2) {
  return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator*(float t, const vec3& v) {
  return vec3(t * v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator/(const vec3& v, float t) {
  return vec3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

inline vec3 operator*(const vec3& v, float t) {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline float dot(const vec3& v1, const vec3& v2) {
  return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
  return vec3(
    v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
    -(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]),
    v1.e[0]*v2.e[1] - v1.e[1] * v2.e[0]); 
}

inline vec3& vec3::operator+=(const vec3& v) {
  e[0] += v.e[0];
  e[1] += v.e[1];
  e[2] += v.e[2];
  return *this;
}

inline vec3& vec3::operator*=(const vec3& v) {
  e[0] *= v.e[0];
  e[1] *= v.e[1]; 
  e[2] *= v.e[2];
  return *this;
}

inline vec3& vec3::operator/=(const vec3& v) {
  e[0] /= v.e[0];
  e[1] /= v.e[1];
  e[2] /= v.e[2];
  return *this;
}

inline vec3& vec3::operator-=(const vec3& v) {
  e[0] -= v.e[0];
  e[1] -= v.e[1];
  e[2] -= v.e[2];
  return *this;
}

inline vec3& vec3::operator*=(const float t) {
  e[0] *= t;
  e[1] *= t;
  e[2] *= t;
  return *this;
}

inline vec3& vec3::operator/=(const float t) {
  float k = 1.0 / t;
  e[0] *= k;
  e[1] *= k;
  e[2] *= k;
  return *this;
}

inline vec3 unit_vector(vec3 v) {
  return v / v.length();
}

vec3 RandomInUnitSphere() {
  // Pick a random point in the unit box until grabbing a point that is inside the unit
  // sphere.
  vec3 p;
  do {
    p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
  } while (p.squared_length() >= 1.0);
  return p;
}

vec3 RandomInUnitDisk() {
  // Pick a random point in the unit 2d box until grabbing a point that is inside the
  // unit disk.
  vec3 p;
  do {
    p = 2.0 * vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
  } while (dot(p, p) >= 1.0);
  return p;
}

vec3 Reflect(const vec3& v, const vec3& normal) {
  return v - 2*dot(v, normal)*normal;
}

bool Refract(const vec3& v, const vec3& normal, float ni_over_nt, vec3* out_refracted) {
  vec3 unit_v = unit_vector(v);
  float v_dot_normal = dot (unit_v, normal);
  float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1 - v_dot_normal*v_dot_normal);
  if (discriminant > 0) {
    *out_refracted = ni_over_nt*(unit_v - normal*v_dot_normal) - normal*sqrt(discriminant);
    return true;
  } else {
    return false;
  }
}
#endif
