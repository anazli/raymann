// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#pragma once

#include "distribution/bxdf.h"

class LambertianBrdf : public Bxdf {
 public:
  // Reflectance spectrum sp, gives fraction of incident scattered light
  LambertianBrdf(const Spectrum& sp);
  ~LambertianBrdf() override = default;
  Spectrum f(const Vec3D& wo, const Vec3D& wi) const override;
  Spectrum rhd(const Vec3D& wo, int num_samples, Vec2D& samples) const override;
  Spectrum rhh(int num_samples, Vec2D& samples1,
               Vec2D& samples2) const override;
  float pdf(const Vec3D& wi, const Vec3D& wo) const override;

 private:
  Spectrum m_spectrum;
};