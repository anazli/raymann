#pragma once

#include "materials/material.h"

class Dielectric : public Material {
 public:
  Dielectric(TexturePtr tex, float refractive_index);
  ~Dielectric() override = default;
  bool scatter(const Ray& r_in, const Intersection& rec, Vec3D& attenuation,
               Ray& scattered) const override;
  static MaterialPtr create(TexturePtr tex, float refractive_index);

 private:
  float m_refractive_index;
};