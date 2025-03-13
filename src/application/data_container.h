#pragma once

#include <any>
#include <map>
#include <optional>

#include "application/parameters.h"
#include "math_utils/math_utils.h"

class DataContainer {
 public:
  DataContainer() = default;
  DataContainer &setProperty(const AppParameters &name, const std::any &value);
  bool addProperty(const AppParameters &name, const std::any &value);
  bool removeProperty(const AppParameters &name);
  bool hasProperty(const AppParameters &name) const;
  template <typename T>
  std::optional<T> getPropertyAs(const AppParameters &name) const {
    if (hasProperty(name)) {
      try {
        return std::any_cast<T>(m_parameters.at(name));
      } catch (const std::bad_any_cast &e) {
        std::cout << e.what() << std::endl;
      }
    }
    return std::nullopt;
  }

 private:
  std::map<AppParameters, std::any> m_parameters;
};
