#include "materials/material.h"

#include "composite/scene_element.h"
#include "material.h"
#include "stochastic/pdf.h"
#include "stochastic/random.h"
#include "stochastic/stochastic_method.h"
#include "tools/orthonormal.h"

Material::Material(TexturePtr tex, const DataContainer& prop)
    : m_tex(std::move(tex)), m_prop(prop) {}

void Material::setTexture(TexturePtr tex) {}

TextureRawPtr Material::getTexture() const { return nullptr; }

void Material::setProperties(const DataContainer& prop) {}

DataContainer Material::getProperties() const { return DataContainer(); }

Vec3D Material::emmit(float u, float v, const Vec3D& p) { return Vec3D(); }

bool Material::isEmissive() const { return false; }

float Material::scatteringPDF(const Ray& r, const IntersectionRecord& record,
                              const Ray& scatteredRay) const {
  return 1.f;
}

std::shared_ptr<StochasticPdf> Material::pdf() const { return m_pdf; }

AppParameters Material::getType() const { return m_type; }

float Material::schlick(float cosine, float ref_idx) const {
  auto r0 = (1.f - ref_idx) / (1.f + ref_idx);
  r0 = r0 * r0;
  return r0 + (1.f - r0) * pow((1.f - cosine), 5);
}

Vec3D Material::reflect(const Vec3D& v, const Vec3D& n) const {
  return v - 2.f * dot(v, n) * n;
}

bool Material::refract(const Vec3D& v, const Vec3D& n, float ni_over_nt,
                       Vec3D& refracted) const {
  auto uv = getUnitVectorOf(v);
  auto dt = dot(uv, n);
  auto discriminant = 1.f - ni_over_nt * ni_over_nt * (1.f - dt * dt);
  if (discriminant > 0) {
    refracted =
        ni_over_nt * (uv - n * dt) - n * static_cast<float>(sqrt(discriminant));
    return true;
  } else
    return false;
}