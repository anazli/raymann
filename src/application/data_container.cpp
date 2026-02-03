#include "application/data_container.h"

DataContainer& DataContainer::setProperty(const App& name,
                                          const std::any& value) {
  if (!addProperty(name, value)) {
    m_parameters[name] = value;
  }
  return *this;
}

bool DataContainer::addProperty(const App& name, const std::any& value) {
  if (!hasProperty(name)) {
    m_parameters[name] = value;
    return true;
  }
  return false;
}

bool DataContainer::removeProperty(const App& name) {
  if (m_parameters.empty()) return false;
  if (hasProperty(name)) {
    m_parameters.erase(name);
    return true;
  }
  return false;
}

bool DataContainer::hasProperty(const App& name) const {
  if (auto it{m_parameters.find(name)}; it != m_parameters.end()) return true;
  return false;
}
