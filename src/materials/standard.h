#pragma once

#include "materials/material.h"

// Standard material. Is meant to be used with phong model renderer.
class StandardMaterial : public Material {
 public:
  StandardMaterial(TexturePtr tex, const DataContainer& prop = DataContainer());
  ~StandardMaterial() override = default;
  void setTexture(TexturePtr tex) override;
  TextureRawPtr getTexture() const override;
  void setProperties(const DataContainer& prop) override;
  DataContainer getProperties() const override;
  bool scatter(const Ray& r_in, const Intersection& rec, Vec3f& attenuation,
               Ray& scattered) const override;

  static MaterialPtr create(TexturePtr tex,
                            const DataContainer& prop = DataContainer());

  DataContainer m_prop;
};