#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.h"

class HitableList : public Hitable {
public:
  HitableList() {}
  HitableList(Hitable** l, int n) { list = l; list_size = n; }
  bool Hit(
    const Ray& r, float t_min, float t_max, HitRecord* out_rec) const;
  Hitable** list;
  int list_size;
};

bool HitableList::Hit(
    const Ray& ray, float t_min, float t_max, HitRecord* out_rec) const {
  HitRecord temp_record;
  bool hit_anything = false;
  float closest_so_far = t_max;
  for (int i = 0; i < list_size; i++) {
    if (list[i]->Hit(ray, t_min, closest_so_far, &temp_record)) {
      hit_anything = true;
      closest_so_far = temp_record.t;
      *out_rec = temp_record;
    }
  }
  return hit_anything;
}
#endif
