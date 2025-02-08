#pragma once

#include "materials/material.h"

class StandardMaterial : public Material {
 public:
  StandardMaterial(TexturePtr tex, const DataContainer& prop = DataContainer());
  ~StandardMaterial() override = default;
  void setTexture(TexturePtr tex) override;
  TextureRawPtr getTexture() const override;
  void setProperties(const DataContainer& prop) override;
  DataContainer getProperties() const override;
  bool scatter(const Ray& r_in, const IntersectionRecord& rec,
               Vec3D& attenuation, Ray& scattered) const override;

  static MaterialPtr create(TexturePtr tex,
                            const DataContainer& prop = DataContainer());
};