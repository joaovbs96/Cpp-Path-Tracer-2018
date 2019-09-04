#pragma once

#include "brdf.hpp"

class Metal : public Brdf {
 public:
  Metal(const float3& albedo) : albedo_(albedo) {}

  virtual float3 sample(const float3& p, const float3& wo, const float3& n,
                        uint& seed) const {
    return reflect(-wo, n);
  }

  virtual float3 eval(const float3& p, const float3& wo, const float3& wi,
                      const float3& n, float& pdf) const {
    return albedo_;
  }

 private:
  float3 albedo_;
};