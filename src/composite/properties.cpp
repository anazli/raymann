#include "composite/properties.h"

Properties::Properties() {
  setProperty(Props::SPHERE_CENTER, Point3f(0.f, 0.f, 0.f))
      .setProperty(Props::SPHERE_RADIUS, 1.0f)
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.9f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::SHININESS, 200.f);
}

Properties& Properties::setProperty(const Props& name, const std::any& value) {
  if (!addProperty(name, value)) {
    m_prop[name] = value;
  }
  return *this;
}

bool Properties::addProperty(const Props& name, const std::any& value) {
  if (!hasProperty(name)) {
    m_prop[name] = value;
    return true;
  }
  return false;
}

bool Properties::removeProperty(const Props& name) {
  if (m_prop.empty()) return false;
  if (hasProperty(name)) {
    m_prop.erase(name);
    return true;
  }
  return false;
}

bool Properties::hasProperty(const Props& name) const {
  if (auto it{m_prop.find(name)}; it != m_prop.end()) return true;
  return false;
}

std::any Properties::getPropertyAsAny(const Props& name) const {
  if (hasProperty(name)) {
    return m_prop.at(name);
  }
  return std::any();
}

int Properties::getPropertyAsInt(const Props& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<int>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0;
}

float Properties::getPropertyAsFloat(const Props& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<float>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0.f;
}

Vec3f Properties::getPropertyAsVec3f(const Props& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<Vec3f>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return Vec3f();
}

Mat4f Properties::getPropertyAsMat4f(const Props& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<Mat4f>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return Mat4f();
}

Point3f Properties::getPropertyAsPoint3f(const Props& name) const {
  if (hasProperty(name)) {
    return std::any_cast<Point3f>(m_prop.at(name));
    try {
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return Point3f();
}
