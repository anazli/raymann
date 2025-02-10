#include "materials/emissive.h"

#include "composite/scene_element.h"

EmissiveMaterial::EmissiveMaterial(TexturePtr tex) : Material(std::move(tex)) {
  m_type = AppParameters::DIFFUSE_LIGHT;
}

bool EmissiveMaterial::scatter(const Ray& r_in, const Intersection& rec,
                               Vec3D& attenuation, Ray& scattered) const {
  return false;
}

Vec3D EmissiveMaterial::emmit(float u, float v, const Vec3D& p) {
  return m_tex->value(u, v, p);
}

bool EmissiveMaterial::isEmissive() const { return true; }

MaterialPtr EmissiveMaterial::create(TexturePtr tex) {
  return std::make_shared<EmissiveMaterial>(std::move(tex));
}