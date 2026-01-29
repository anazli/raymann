#pragma once

#include "materials/material.h"

class EmissiveMaterial : public Material {
 public:
  EmissiveMaterial(TexturePtr tex);
  ~EmissiveMaterial() override = default;
  bool scatter(const Ray& r_in, const Intersection& rec, Vec3f& attenuation,
               Ray& scattered) const override;
  Vec3f emmit(float u = 0.f, float v = 0.f, const Vec3f& p = Vec3f()) override;
  bool isEmissive() const override;
  static MaterialPtr create(TexturePtr tex);
};