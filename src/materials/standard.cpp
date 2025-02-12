#include "materials/standard.h"

#include "composite/scene_element.h"

StandardMaterial::StandardMaterial(TexturePtr tex, const DataContainer& prop)
    : Material(std::move(tex)), m_prop(prop) {
  // Default properties of standard material
  if (!m_prop.hasProperty(AppParameters::AMBIENT))
    m_prop.setProperty(AppParameters::AMBIENT, 0.1f);
  if (!m_prop.hasProperty(AppParameters::DIFFUSE))
    m_prop.setProperty(AppParameters::DIFFUSE, 0.9f);
  if (!m_prop.hasProperty(AppParameters::SPECULAR))
    m_prop.setProperty(AppParameters::SPECULAR, 0.9f);
  if (!m_prop.hasProperty(AppParameters::SHININESS))
    m_prop.setProperty(AppParameters::SHININESS, 200.f);
  if (!m_prop.hasProperty(AppParameters::REFLECTION))
    m_prop.setProperty(AppParameters::REFLECTION, 0.f);
  if (!m_prop.hasProperty(AppParameters::TRANSPARENCY))
    m_prop.setProperty(AppParameters::TRANSPARENCY, 0.f);
  if (!m_prop.hasProperty(AppParameters::REFRACTIVE_INDEX))
    m_prop.setProperty(AppParameters::REFRACTIVE_INDEX, 1.f);
}

void StandardMaterial::setTexture(TexturePtr tex) { m_tex = std::move(tex); }

TextureRawPtr StandardMaterial::getTexture() const { return m_tex.get(); }

void StandardMaterial::setProperties(const DataContainer& prop) {
  m_prop = prop;
}

DataContainer StandardMaterial::getProperties() const { return m_prop; }

bool StandardMaterial::scatter(const Ray& r_in, const Intersection& rec,
                               Vec3D& attenuation, Ray& scattered) const {
  return false;
}

MaterialPtr StandardMaterial::create(TexturePtr tex,
                                     const DataContainer& prop) {
  return std::make_shared<StandardMaterial>(std::move(tex), prop);
}