#include "materials/standard.h"

#include "composite/scene_element.h"

StandardMaterial::StandardMaterial(TexturePtr tex, const DataContainer& prop)
    : Material(std::move(tex)), m_prop(prop) {
  // Default properties of standard material
  if (!m_prop.hasProperty(App::AMBIENT)) m_prop.setProperty(App::AMBIENT, 0.1f);
  if (!m_prop.hasProperty(App::DIFFUSE)) m_prop.setProperty(App::DIFFUSE, 0.9f);
  if (!m_prop.hasProperty(App::SPECULAR))
    m_prop.setProperty(App::SPECULAR, 0.9f);
  if (!m_prop.hasProperty(App::SHININESS))
    m_prop.setProperty(App::SHININESS, 200.f);
  if (!m_prop.hasProperty(App::REFLECTION))
    m_prop.setProperty(App::REFLECTION, 0.f);
  if (!m_prop.hasProperty(App::TRANSPARENCY))
    m_prop.setProperty(App::TRANSPARENCY, 0.f);
  if (!m_prop.hasProperty(App::REFRACTIVE_INDEX))
    m_prop.setProperty(App::REFRACTIVE_INDEX, 1.f);
}

void StandardMaterial::setTexture(TexturePtr tex) { m_tex = std::move(tex); }

TextureRawPtr StandardMaterial::getTexture() const { return m_tex.get(); }

void StandardMaterial::setProperties(const DataContainer& prop) {
  m_prop = prop;
}

DataContainer StandardMaterial::getProperties() const { return m_prop; }

bool StandardMaterial::scatter(const Ray& r_in, const Intersection& rec,
                               Vec3f& attenuation, Ray& scattered) const {
  return false;
}

MaterialPtr StandardMaterial::create(TexturePtr tex,
                                     const DataContainer& prop) {
  return std::make_shared<StandardMaterial>(std::move(tex), prop);
}