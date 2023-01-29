#include "composite/properties.h"

Properties& Properties::setProperty(const PropertyNames& name,
                                    const std::any& value) {
  if (!addProperty(name, value)) {
    m_prop[name] = value;
  }
  return *this;
}

bool Properties::addProperty(const PropertyNames& name, const std::any& value) {
  if (!hasProperty(name)) {
    m_prop[name] = value;
    return true;
  }
  return false;
}

bool Properties::removeProperty(const PropertyNames& name) {
  if (m_prop.empty()) return false;
  if (hasProperty(name)) {
    m_prop.erase(name);
    return true;
  }
  return false;
}

bool Properties::hasProperty(const PropertyNames& name) const {
  auto it = m_prop.find(name);
  if (it != m_prop.end()) return true;
  return false;
}

std::any Properties::getPropertyAsAny(const PropertyNames& name) const {
  if (hasProperty(name)) {
    return m_prop.at(name);
  }
  return std::any();
}

int Properties::getPropertyAsInt(const PropertyNames& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<int>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0;
}

float Properties::getPropertyAsFloat(const PropertyNames& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<float>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0.f;
}

Vec3f Properties::getPropertyAsVec3f(const PropertyNames& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<Vec3f>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return Vec3f();
}

Mat4f Properties::getPropertyAsMat4f(const PropertyNames& name) const {
  if (hasProperty(name)) {
    try {
      return std::any_cast<Mat4f>(m_prop.at(name));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return Mat4f();
}

Point3f Properties::getPropertyAsPoint3f(const PropertyNames& name) const {
  if (hasProperty(name)) {
    return std::any_cast<Point3f>(m_prop.at(name));
    try {
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return Point3f();
}
