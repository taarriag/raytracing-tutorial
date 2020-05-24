#include "camera.h"
#include "vec3.h"
#include "ray.h"
#include "hitable_list.h"
#include "sphere.h"
#include "float.h"

vec3 Color(const Ray& r, Hitable* world, int depth) {
  HitRecord hit_record;
  if (world->Hit(r, 0.001, MAXFLOAT, &hit_record)) {
    Ray scattered;
    vec3 attenuation;
    if (depth < 50 && hit_record.material_ptr->Scatter(r, hit_record, &attenuation, &scattered)) {
      return attenuation * Color(scattered, world, depth + 1);
    } else {
      return vec3(0,0,0);
    }
  } else {
    vec3 unit_direction = unit_vector(r.direction());
    // map -1.0 < y < 1.0 to 0.0 < t < 1.0.
    float t = 0.5 * (unit_direction.y() + 1.0);
    // White on t = 0.0, blue on t = 1.0
    return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
  }
}
int main() {
  int nx = 200;
  int ny = 100;
  int ns = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  Hitable* list[5];
  list[0] = new Sphere(vec3(0,0,-1), 0.5, new Lambertian(vec3(0.1, 0.2, 0.5)));
  list[1] = new Sphere(vec3(0, -100.5, -1), 100, new Lambertian(vec3(0.8, 0.8, 0.0)));
  list[2] = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), /*fuzz=*/0.05));
  list[3] = new Sphere(vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
  list[4] = new Sphere(vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
  Hitable* world = new HitableList(list, 5);
  Camera camera;
  for (int j = ny - 1; j >= 0; --j) {
    for (int i = 0; i < nx; ++i) {
      vec3 col(0,0,0);
      // Multisampling
      for (int s = 0; s < ns; s++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        Ray r = camera.GetRay(u, v);
        col += Color(r, world, /*depth=*/0);
      }
      col /= float(ns);
      // Image Viewers assume the image has been gamma corrected.
      // Apply Gamma Correction here, raising the color to the power
      // 1/gamma where gamma = 2.
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
   }
  }
}
