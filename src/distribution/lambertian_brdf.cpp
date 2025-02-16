// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#include "distribution/lambertian_brdf.h"

#include "lambertian_brdf.h"
#include "tools/tools.h"

LambertianBrdf::LambertianBrdf(const Spectrum& sp)
    : Bxdf(BxdfType(BxdfType::BSDF_REFLECTION | BxdfType::BSDF_DIFFUSE)) {}

Spectrum LambertianBrdf::f(const Vec3D& wo, const Vec3D& wi) const {
  return m_spectrum * InvPI;
}

Spectrum LambertianBrdf::rhd(const Vec3D& wo, int num_samples,
                             Vec2D& samples) const {
  return m_spectrum;
}

Spectrum LambertianBrdf::rhh(int num_samples, Vec2D& samples1,
                             Vec2D& samples2) const {
  return m_spectrum;
}
