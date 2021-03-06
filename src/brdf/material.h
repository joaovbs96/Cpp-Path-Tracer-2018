#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "../texture/texture.h"

#include "../pdf/pdf.h"
#include "../ray.h"
#include "hitable.h"

vec3 random_in_unit_sphere() {
  float z = RFG() * 2.0f - 1.0f;
  float t = RFG() * 2.0f * 3.1415926f;
  float r = sqrt((0.0f > (1.0f - z * z) ? 0.0f : (1.0f - z * z)));
  float x = r * cos(t);
  float y = r * sin(t);
  vec3 res(x, y, z);
  res *= pow(RFG(), 1.0f / 3.0f);
  return res;
}

vec3 random_on_unit_sphere() {
  float z = RFG() * 2.0f - 1.0f;
  float t = RFG() * 2.0f * 3.1415926f;
  float r = sqrt((0.0f > (1.0f - z * z) ? 0.0f : (1.0f - z * z)));
  float x = r * cos(t);
  float y = r * sin(t);
  vec3 res(x, y, z);
  res *= pow(RFG(), 1.0f / 3.0f);
  return unit_vector(res);
}

struct scatter_record {
  Ray specular_ray;
  bool is_specular;
  vec3 attenuation;
  pdf* pdf_ptr;
};

class material {
 public:
  virtual bool scatter(const Ray& r_in, const hit_record& hrec,
                       scatter_record& srec) const {
    return false;
  }

  virtual float scattering_pdf(const Ray& r_in, const hit_record& rec,
                               const Ray& scattered) const {
    return 0.0;
  }

  virtual vec3 emitted(const Ray& r_in, const hit_record& rec, float u, float v,
                       const vec3& p) const {
    return vec3(0, 0, 0);
  }
};

vec3 reflect(const vec3& v, const vec3& n) { return v - 2.0f * dot(v, n) * n; }

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
  vec3 uv = unit_vector(v);
  float dt = dot(uv, n);
  float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);

  if (discriminant > 0.0f) {
    refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
    return true;
  } else {
    return false;
  }
}

float schlick(float cosine, float ref_idx) {
  float r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

#endif