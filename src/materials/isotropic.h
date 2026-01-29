#pragma once

#include "materials/material.h"

class Isotropic : public Material {
 public:
  Isotropic(TexturePtr tex);
  ~Isotropic() override = default;
  bool scatter(const Ray& r_in, const Intersection& rec, Vec3f& attenuation,
               Ray& scattered) const override;
  float scatteringPDF(const Ray& r, const Intersection& record,
                      const Ray& scatteredRay) const override;
  static MaterialPtr create(TexturePtr tex);
};