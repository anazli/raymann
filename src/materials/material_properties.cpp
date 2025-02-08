#include "materials/material_properties.h"

DataContainer::DataContainer() {
  setProperty(Properties::AMBIENT, 0.1f)
      .setProperty(Properties::DIFFUSE, 0.9f)
      .setProperty(Properties::SPECULAR, 0.9f)
      .setProperty(Properties::SHININESS, 200.f)
      .setProperty(Properties::REFLECTION, 0.f)
      .setProperty(Properties::TRANSPARENCY, 0.f)
      .setProperty(Properties::REFRACTIVE_INDEX, 1.f);
}

DataContainer& DataContainer::setProperty(const Properties& name,
                                          const std::any& value) {
  if (!addProperty(name, value)) {
    m_prop[name] = value;
  }
  return *this;
}

bool DataContainer::addProperty(const Properties& name, const std::any& value) {
  if (!hasProperty(name)) {
    m_prop[name] = value;
    return true;
  }
  return false;
}

bool DataContainer::removeProperty(const Properties& name) {
  if (m_prop.empty()) return false;
  if (hasProperty(name)) {
    m_prop.erase(name);
    return true;
  }
  return false;
}

bool DataContainer::hasProperty(const Properties& name) const {
  if (auto it{m_prop.find(name)}; it != m_prop.end()) return true;
  return false;
}
