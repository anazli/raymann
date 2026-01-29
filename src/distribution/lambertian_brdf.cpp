// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#include "distribution/lambertian_brdf.h"

#include "lambertian_brdf.h"
#include "math_utils/math_utils.h"

LambertianBrdf::LambertianBrdf(const Spectrum& sp)
    : Bxdf(BxdfType(BxdfType::BSDF_REFLECTION | BxdfType::BSDF_DIFFUSE)) {}

Spectrum LambertianBrdf::f(const Vec3f& wo, const Vec3f& wi) const {
  return m_spectrum * InvPI;
}

Spectrum LambertianBrdf::rhd(const Vec3f& wo, int num_samples,
                             Vec2D& samples) const {
  return m_spectrum;
}

Spectrum LambertianBrdf::rhh(int num_samples, Vec2D& samples1,
                             Vec2D& samples2) const {
  return m_spectrum;
}
