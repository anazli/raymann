#include "application/data_container.h"

DataContainer& DataContainer::setProperty(const AppParameters& name,
                                          const std::any& value) {
  if (!addProperty(name, value)) {
    m_parameters[name] = value;
  }
  return *this;
}

bool DataContainer::addProperty(const AppParameters& name,
                                const std::any& value) {
  if (!hasProperty(name)) {
    m_parameters[name] = value;
    return true;
  }
  return false;
}

bool DataContainer::removeProperty(const AppParameters& name) {
  if (m_parameters.empty()) return false;
  if (hasProperty(name)) {
    m_parameters.erase(name);
    return true;
  }
  return false;
}

bool DataContainer::hasProperty(const AppParameters& name) const {
  if (auto it{m_parameters.find(name)}; it != m_parameters.end()) return true;
  return false;
}
