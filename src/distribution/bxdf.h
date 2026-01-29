// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#pragma once

#include <memory>

#include "color/spectrum.h"
#include "math_utils/vec2.h"

// Local coordinate system
// normal (0,0,1)
enum BxdfType {
  BSDF_REFLECTION = 1,
  BSDF_DIFFUSE = 2,
};

class Bxdf {
 public:
  virtual ~Bxdf() = default;
  // value of distribution for pair of directions
  virtual Spectrum f(const Vec3f& wo, const Vec3f& wi) const = 0;
  virtual Spectrum sampleF(const Vec3f& wo, Vec3f& wi, const Vec2D& p,
                           float& pdf_val) const;
  // Hemispherical-directional reflectance
  virtual Spectrum rhd(const Vec3f& wo, int num_samples, Vec2D& samples) const;
  // Hemispherical-hemispherical reflectance
  virtual Spectrum rhh(int num_samples, Vec2D& samples1, Vec2D& samples2) const;
  virtual float pdf(const Vec3f& wi, const Vec3f& wo) const;

 protected:
  explicit Bxdf(const BxdfType& type);
  BxdfType m_bsdf_type;
};

using BxdfPtr = std::shared_ptr<Bxdf>;