#include "composite/properties.h"

/*Properties& Properties::reset() {
  m_object_trans = Mat4f();
  m_pattern_trans = Mat4f();
  m_color = Vec3f();
  m_pattern_colorA = Vec3f();
  m_pattern_colorB = Vec3f();
  m_ambient = 0.1f;
  m_diffuse = 0.9f;
  m_specular = 0.9f;
  m_shininess = 200.f;
  m_reflective = 0.f;
  m_sphere_center = Point3f(0.f, 0.f, 0.f);
  m_sphere_radius = 1.f;

  return *this;
}*/

Properties& Properties::setProperty(const std::string& name,
                                    const std::any& value) {
  if (!name.empty()) m_prop[name] = value;
  return *this;
}

bool Properties::addProperty(const std::string& name, const std::any& value) {
  if (name.empty() || hasProperty(name)) return false;

  m_prop[name] = value;
  return true;
}

bool Properties::removeProperty(const std::string& name) { return true; }

bool Properties::hasProperty(const std::string& name) {
  auto it = m_prop.find(name);
  if (it != m_prop.end()) return true;
  return false;
}
