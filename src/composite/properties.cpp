#include "composite/properties.h"

Properties& Properties::setProperty(const std::string& name,
                                    const std::any& value) {
  if (!addProperty(name, value)) {
    m_prop[name] = value;
  }
  return *this;
}

bool Properties::addProperty(const std::string& name, const std::any& value) {
  if (!hasProperty(name) && !name.empty()) {
    m_prop[name] = value;
    return true;
  }
  return false;
}

bool Properties::removeProperty(const std::string& name) { return true; }

bool Properties::hasProperty(const std::string& name) {
  auto it = m_prop.find(name);
  if (it != m_prop.end()) return true;
  return false;
}

std::any Properties::getPropertyAsAny(const std::string& name) {
  if (hasProperty(name)) {
    return m_prop[name];
  }
  return std::any();
}

int Properties::getPropertyAsInt(const std::string& name) {
  if (hasProperty(name)) {
    try {
      return std::any_cast<int>(m_prop[name]);
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
      std::cout << "Check property:" << name << " type!" << std::endl;
    }
  }
  return 0;
}

float Properties::getPropertyAsFloat(const std::string& name) {
  if (hasProperty(name)) {
    try {
      return std::any_cast<float>(m_prop[name]);
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
      std::cout << "Check property:" << name << " type!" << std::endl;
    }
  }
  return 0.f;
}

Vec3f Properties::getPropertyAsVec3f(const std::string& name) {
  if (hasProperty(name)) {
    try {
      return std::any_cast<Vec3f>(m_prop[name]);
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
      std::cout << "Check property:" << name << " type!" << std::endl;
    }
  }
  return Vec3f();
}

Mat4f Properties::getPropertyAsMat4f(const std::string& name) {
  if (hasProperty(name)) {
    try {
      return std::any_cast<Mat4f>(m_prop[name]);
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
      std::cout << "Check property:" << name << " type!" << std::endl;
    }
  }
  return Mat4f();
}

Point3f Properties::getPropertyAsPoint3f(const std::string& name) {
  if (hasProperty(name)) {
    return std::any_cast<Point3f>(m_prop[name]);
    try {
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
      std::cout << "Check property:" << name << " type!" << std::endl;
    }
  }
  return Point3f();
}
