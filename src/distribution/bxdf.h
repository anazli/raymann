#pragma once

#include "color/spectrum.h"
#include "tools/vec2.h"

enum BxdfType {
  BSDF_REFLECTION = 1,
  BSDF_DIFFUSE = 2,
};

class Bxdf {
 public:
  virtual ~Bxdf() = default;
  // value of distribution for pair of directions
  virtual Spectrum f(const Vec3D& wo, const Vec3D& wi) const = 0;
  // Hemispherical-directional reflectance
  virtual Spectrum rhd(const Vec3D& wo, int num_samples, Vec2D& samples) const;
  // Hemispherical-hemispherical reflectance
  virtual Spectrum rhh(int num_samples, Vec2D& samples1, Vec2D& samples2) const;
  virtual float pdf(const Vec3D& wi, const Vec3D& wo) const;

 protected:
  explicit Bxdf(const BxdfType& type);
  BxdfType m_bsdf_type;
};