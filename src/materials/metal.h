#pragma once

#include "materials/material.h"

class Metal : public Material {
 public:
  Metal(TexturePtr tex, float fuzz);
  ~Metal() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  static MaterialPtr create(TexturePtr tex, float fuzz);

 private:
  float m_fuzz;
};