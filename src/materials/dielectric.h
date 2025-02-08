#pragma once

#include "materials/material.h"

class Dielectric : public Material {
 public:
  Dielectric(TexturePtr tex, const DataContainer& prop = DataContainer());
  ~Dielectric() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  static MaterialPtr create(TexturePtr tex,
                            const DataContainer& prop = DataContainer());

 private:
  float ref_idx;
};