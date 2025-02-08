#include "materials/standard.h"

#include "composite/scene_element.h"

StandardMaterial::StandardMaterial(TexturePtr tex, const DataContainer& prop)
    : Material(std::move(tex), prop) {}

void StandardMaterial::setTexture(TexturePtr tex) { m_tex = std::move(tex); }

TextureRawPtr StandardMaterial::getTexture() const { return m_tex.get(); }

void StandardMaterial::setProperties(const DataContainer& prop) {
  m_prop = prop;
}

DataContainer StandardMaterial::getProperties() const { return m_prop; }

bool StandardMaterial::scatter(const Ray& r_in, const IntersectionRecord& rec,
                               Vec3D& attenuation, Ray& scattered) const {
  return false;
}

MaterialPtr StandardMaterial::create(TexturePtr tex,
                                     const DataContainer& prop) {
  return std::make_shared<StandardMaterial>(std::move(tex), prop);
}