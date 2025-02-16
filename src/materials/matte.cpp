#include "materials/matte.h"

#include "distribution/lambertian_brdf.h"
#include "materials/bsdf.h"
#include "matte.h"

Matte::Matte(TexturePtr texture) : BaseMaterial(std::move(texture)) {}

void Matte::evaluateScattering(const Intersection& record) {
  auto bsdf = std::make_shared<Bsdf>(record);
  Spectrum spectrum = m_texture->value(record).clamp();
  if (!spectrum.isBlack()) {
    if (m_roughness == 0.f) {
      auto lambertian_reflection = std::make_shared<LambertianBrdf>(spectrum);
    }
  }
}
