#pragma once

#include <any>
#include <iostream>
#include <map>
#include <optional>

#include "application/parameters.h"

class DataContainer {
 public:
  DataContainer() = default;
  DataContainer &setProperty(const App &name, const std::any &value);
  bool addProperty(const App &name, const std::any &value);
  bool removeProperty(const App &name);
  bool hasProperty(const App &name) const;
  template <typename T>
  std::optional<T> getPropertyAs(const App &name) const {
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
  std::map<App, std::any> m_parameters;
};
