#include "materials/emissive.h"

#include "composite/scene_element.h"

EmissiveMaterial::EmissiveMaterial(TexturePtr tex, const DataContainer& prop)
    : Material(std::move(tex), prop) {
  m_type = MaterialType::DIFFUSE_LIGHT;
}

bool EmissiveMaterial::scatter(const Ray& r_in, const IntersectionRecord& rec,
                               Vec3D& attenuation, Ray& scattered) const {
  return false;
}

Vec3D EmissiveMaterial::emmit(float u, float v, const Vec3D& p) {
  return m_tex->value(u, v, p);
}

bool EmissiveMaterial::isEmissive() const { return true; }

MaterialPtr EmissiveMaterial::create(TexturePtr tex,
                                     const DataContainer& prop) {
  return std::make_shared<EmissiveMaterial>(std::move(tex), prop);
}