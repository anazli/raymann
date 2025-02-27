#pragma once

#include "materials/material.h"

class EmissiveMaterial : public Material {
 public:
  EmissiveMaterial(TexturePtr tex);
  ~EmissiveMaterial() override = default;
  bool scatter(const Ray& r_in, const Intersection& rec, Vec3D& attenuation,
               Ray& scattered) const override;
  Vec3D emmit(float u = 0.f, float v = 0.f, const Vec3D& p = Vec3D()) override;
  bool isEmissive() const override;
  static MaterialPtr create(TexturePtr tex);
};