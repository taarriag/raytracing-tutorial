#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"

class Camera { 
  public:
    // vfov is top to bottom in degrees.
    // aspect_ratio is width/height.
    Camera(const vec3& lookfrom, const vec3& lookat, const vec3& vup, 
           float vfov_degrees, float aspect_ratio) {
      vec3 u, v, w;  
      float theta = vfov_degrees * M_PI/180;
      float half_height = tan(theta/2);
      float half_width = aspect_ratio * half_height;
      origin = lookfrom;
      w = unit_vector(lookfrom-lookat);
      u = unit_vector(cross(vup, w));
      v = cross(w, u);
      lower_left_corner = origin - half_width*u - half_height*v - w;
      horizontal = 2*half_width*u;
      vertical = 2*half_height*v;
    }
    Ray GetRay(float u, float v) {
      return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
#endif
