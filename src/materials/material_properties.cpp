#include "materials/material_properties.h"

MaterialProperties::MaterialProperties() {
  setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.9f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::SHININESS, 200.f)
      .setProperty(Props::REFLECTION, 0.f)
      .setProperty(Props::TRANSPARENCY, 0.f)
      .setProperty(Props::REFRACTIVE_INDEX, 1.f);
}

MaterialProperties& MaterialProperties::setProperty(const Props& name,
                                                    const std::any& value) {
  if (!addProperty(name, value)) {
    m_prop[name] = value;
  }
  return *this;
}

bool MaterialProperties::addProperty(const Props& name, const std::any& value) {
  if (!hasProperty(name)) {
    m_prop[name] = value;
    return true;
  }
  return false;
}

bool MaterialProperties::removeProperty(const Props& name) {
  if (m_prop.empty()) return false;
  if (hasProperty(name)) {
    m_prop.erase(name);
    return true;
  }
  return false;
}

bool MaterialProperties::hasProperty(const Props& name) const {
  if (auto it{m_prop.find(name)}; it != m_prop.end()) return true;
  return false;
}

std::any MaterialProperties::getPropertyAsAny(const Props& name) const {
  if (hasProperty(name)) {
    return m_prop.at(name);
  }
  return std::any();
}

int MaterialProperties::getPropertyAsInt(const Props& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<int>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0;
}

float MaterialProperties::getPropertyAsFloat(const Props& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<float>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0.f;
}

Vec3D MaterialProperties::getPropertyAsVec3D(const Props& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<Vec3D>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return Vec3D();
}

Mat4D MaterialProperties::getPropertyAsMat4D(const Props& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<Mat4D>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return Mat4D();
}

Point3D MaterialProperties::getPropertyAsPoint3D(const Props& name) const {
  if (hasProperty(name)) {
    return std::any_cast<Point3D>(m_prop.at(name));
    try {
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return Point3D();
}
