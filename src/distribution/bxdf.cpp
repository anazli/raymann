#include "bxdf.h"

Spectrum Bxdf::rhd(const Vec3D& wo, int num_samples, Vec2D& samples) const {
  return Spectrum();
}

Spectrum Bxdf::rhh(int num_samples, Vec2D& samples1, Vec2D& samples2) const {
  return Spectrum();
}

float Bxdf::pdf(const Vec3D& wi, const Vec3D& wo) const { return 0.0f; }

Bxdf::Bxdf(const BxdfType& type) : m_bsdf_type(type) {}