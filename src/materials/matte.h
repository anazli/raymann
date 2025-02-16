#pragma once

#include "materials/material.h"

class Matte : public BaseMaterial {
 public:
  Matte(TexturePtr texture);
  ~Matte() override = default;
  void evaluateScattering(const Intersection& record) override;

 private:
  float m_roughness{};
};