#pragma once

#include "materials/material.h"

class Isotropic : public Material {
 public:
  Isotropic(TexturePtr tex);
  ~Isotropic() override = default;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;
  float scatteringPDF(const Ray& r, const IntersectionRecord& record,
                      const Ray& scatteredRay) const override;
  static MaterialPtr create(TexturePtr tex);
};