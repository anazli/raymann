#pragma once

#include "materials/material.h"

class Metal : public Material {
 public:
  Metal(TexturePtr tex, const DataContainer& prop = DataContainer());
  ~Metal() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  static MaterialPtr create(TexturePtr tex,
                            const DataContainer& prop = DataContainer());

 private:
  float m_fuzz;
};