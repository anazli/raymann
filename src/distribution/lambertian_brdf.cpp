#include "distribution/lambertian_brdf.h"

#include "tools/tools.h"

LambertianBrdf::LambertianBrdf(const Spectrum& sp)
    : Bxdf(BxdfType(BxdfType::BSDF_REFLECTION | BxdfType::BSDF_DIFFUSE)) {}

Spectrum LambertianBrdf::f(const Vec3D& wo, const Vec3D& wi) const {
  return m_spectrum * InvPI;
}
