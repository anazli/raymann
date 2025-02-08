#include "materials/isotropic.h"

#include "composite/scene_element.h"
#include "stochastic/pdf.h"
#include "stochastic/random.h"
#include "stochastic/stochastic_method.h"

Isotropic::Isotropic(TexturePtr tex, const DataContainer& prop)
    : Material(std::move(tex), prop) {
  m_pdf = std::make_shared<SpherePdf>();
  m_type = AppParameters::ISOTROPIC;
}

bool Isotropic::scatter(const Ray& r_in, const IntersectionRecord& rec,
                        Vec3D& attenuation, Ray& scattered) const {
  scattered =
      Ray(rec.point(r_in), getUnitVectorOf(Random::randomVectorOnUnitSphere()));
  attenuation = m_tex->value(0.f, 0.f, Vec3D());
  return true;
}

float Isotropic::scatteringPDF(const Ray& r, const IntersectionRecord& record,
                               const Ray& scatteredRay) const {
  return 1.f / (4.f * PI);
}

MaterialPtr Isotropic::create(TexturePtr tex, const DataContainer& prop) {
  return std::make_shared<Isotropic>(std::move(tex), prop);
}