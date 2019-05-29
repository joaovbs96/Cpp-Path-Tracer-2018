#ifndef BVHNODEH
#define BVHNODEH

#include "hitable.h"

class bvh_node : public hitable {
 public:
  bvh_node() {}

  bvh_node(hitable** l, int n, float time0, float time1);

  virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
  virtual bool bounding_box(float t0, float t1, aabb& box) const;
  virtual float pdf_value(const vec3& o, const vec3& v) const;
  virtual vec3 random(const vec3& o) const;

  hitable *left, *right;
  aabb box;
};

bool bvh_node::hit(const Ray& r, float tmin, float tmax,
                   hit_record& rec) const {
  if (box.hit(r, tmin, tmax)) {
    hit_record left_rec, right_rec;

    bool hit_left = left->hit(r, tmin, tmax, left_rec);
    bool hit_right = right->hit(r, tmin, tmax, right_rec);

    if (hit_left && hit_right) {
      rec = left_rec.t < right_rec.t ? left_rec : right_rec;
      return true;
    } else if (hit_left) {
      rec = left_rec;
      return true;
    } else if (hit_right) {
      rec = right_rec;
      return true;
    } else
      return false;
  }

  return false;
}

bool bvh_node::bounding_box(float t0, float t1, aabb& b) const {
  b = box;
  return true;
}

int box_x_compare(const void* a, const void* b) {
  aabb box_left, box_right;

  hitable* ah = *(hitable**)a;
  hitable* bh = *(hitable**)b;

  if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
    std::cerr << "no bounding box in bvh_node constructor" << std::endl;

  if (box_left.min().x() - box_right.min().x() < 0.0)
    return -1;
  else
    return 1;
}

int box_y_compare(const void* a, const void* b) {
  aabb box_left, box_right;

  hitable* ah = *(hitable**)a;
  hitable* bh = *(hitable**)b;

  if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
    std::cerr << "no bounding box in bvh_node constructor" << std::endl;

  if (box_left.min().y() - box_right.min().y() < 0.0)
    return -1;
  else
    return 1;
}

int box_z_compare(const void* a, const void* b) {
  aabb box_left, box_right;

  hitable* ah = *(hitable**)a;
  hitable* bh = *(hitable**)b;

  if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
    std::cerr << "no bounding box in bvh_node constructor" << std::endl;

  if (box_left.min().z() - box_right.min().z() < 0.0)
    return -1;
  else
    return 1;
}

bvh_node::bvh_node(hitable** l, int n, float time0, float time1) {
  int axis = int(3 * RFG());

  if (axis == 0)
    qsort(l, n, sizeof(hitable*), box_x_compare);
  else if (axis == 1)
    qsort(l, n, sizeof(hitable*), box_y_compare);
  else
    qsort(l, n, sizeof(hitable*), box_z_compare);

  if (n == 1) {
    left = right = l[0];
  } else if (n == 2) {
    left = l[0];
    right = l[1];
  } else {
    left = new bvh_node(l, n / 2, time0, time1);
    right = new bvh_node(l + n / 2, n - n / 2, time0, time1);
  }

  aabb box_left, box_right;
  if (!left->bounding_box(time0, time1, box_left) ||
      !right->bounding_box(time0, time1, box_right))
    std::cerr << "no bounding box in bvh_node constructor" << std::endl;

  box = surrounding_box(box_left, box_right);
}

float bvh_node::pdf_value(const vec3& o, const vec3& v) const {
  return (left->pdf_value(o, v) + right->pdf_value(o, v)) / 2;
}

vec3 bvh_node::random(const vec3& o) const {
  if (RFG() > 0.5)  // right
    return right->random(o);
  else  // left
    return left->random(o);
}

#endif  // !BVHNODEH
