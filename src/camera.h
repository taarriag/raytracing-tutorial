#ifndef CAMERA_H
#define CAMERA_H
#include "ray.h"

class Camera { 
  public:
    // vfov is top to bottom in degrees.
    // aspect_ratio is width/height.
    Camera(const vec3& lookfrom, const vec3& lookat, const vec3& world_up, 
           float vfov_degrees, float aspect_ratio, float aperture, 
           float focus_dist) {
      lens_radius = aperture/2;
      vec3 u, v, w;  
      float theta = vfov_degrees * M_PI/180;
      float half_height = tan(theta/2);
      float half_width = aspect_ratio * half_height;
      origin = lookfrom;
      backward = unit_vector(lookfrom-lookat);
      right = unit_vector(cross(world_up, backward));
      up = cross(backward, right);
      lower_left_corner = origin - half_width*focus_dist*right - half_height*focus_dist*up 
                                 - focus_dist*backward;
      horizontal = 2*half_width*focus_dist*right;
      vertical = 2*half_height*focus_dist*up;
    }
    Ray GetRay(float u, float v) {
      vec3 rd = lens_radius * RandomInUnitDisk();
      vec3 offset = right * rd.x() + up * rd.y();
      return Ray(origin + offset, lower_left_corner + u * horizontal + v * vertical 
                 - origin - offset);
    }
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 right, up, backward;
    float lens_radius;
};

#endif
