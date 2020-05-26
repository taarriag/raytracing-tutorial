#include "camera.h"
#include "vec3.h"
#include "ray.h"
#include "hitable_list.h"
#include "sphere.h"
#include "float.h"

constexpr int WIDTH_IN_PIXELS = 200;
constexpr int HEIGHT_IN_PIXELS = 100;
constexpr int MAX_RAYCAST_DEPTH = 50;
constexpr int SAMPLES_PER_PIXEL = 100;

vec3 Color(const Ray& r, Hitable* world, int depth) {
  HitRecord hit_record;
  if (world->Hit(r, 0.001, MAXFLOAT, &hit_record)) {
    Ray scattered;
    vec3 attenuation;
    if (depth < MAX_RAYCAST_DEPTH && hit_record.material_ptr->Scatter(r, hit_record, &attenuation, &scattered)) {
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

Hitable* RandomScene() {
  int n = 500;
  Hitable** list = new Hitable*[n + 1];
  list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5, 0.5, 0.5)));
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = drand48();
      vec3 center(a + 0.9*drand48(), 0.2, b+0.9*drand48());
      if ((center - vec3(4, 0.2, 0)).length() < 0.9) {
        continue;
      }
      // Diffuse, Metal or Glass.
      if (choose_mat < 0.8) {
        list[i++] = new Sphere(center, 0.2, 
          new Lambertian(
            /*albedo=*/vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
      } else if (choose_mat < 0.95) {
        list[i++] = new Sphere(center, 0.2, 
          new Metal(/*albedo=*/vec3(0.5*(1 + drand48()),
                         0.5*(1 + drand48()),
                         0.5*(1 + drand48())),
                    /*fuzz=*/0.5*(1 + drand48())));
      } else {
        list[i++] = new Sphere(center, 0.2, new Dielectric(/*ref_idx=*/1.5));
      }
    }
  }

  list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5));
  list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(vec3(0.4, 0.2, 0.1)));
  list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));
  return new HitableList(list, i);
}


int main() {
  int nx = WIDTH_IN_PIXELS;
  int ny = HEIGHT_IN_PIXELS;
  int ns = SAMPLES_PER_PIXEL;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  Hitable* world = RandomScene();
  vec3 lookfrom(13,2,3);
  vec3 lookat(0,0,0);
  float dist_to_focus = 10.0;
  float aperture = 0.1;
  Camera camera(
    lookfrom,
    lookat,
    /*vup=*/vec3(0, 1, 0),
    /*vfov=*/20,
    /*aspect_ratio=*/float(nx)/float(ny),
    aperture,
    dist_to_focus);

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
