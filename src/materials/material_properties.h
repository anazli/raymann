#pragma once

#include <any>
#include <map>
#include <optional>

#include "tools/tools.h"

class MaterialProperties {
 public:
  enum Properties {
    COLOR,             // Vec3D
    AMBIENT,           // float [0,1]
    DIFFUSE,           // float [0,1]
    SPECULAR,          // float [0,1]
    SHININESS,         // float [0,..]
    REFLECTION,        // float [0,1]
    TRANSPARENCY,      // float [0,1]
    REFRACTIVE_INDEX,  // float [0,..]
    FUZZ               // float [0,1]
  };
  MaterialProperties();
  MaterialProperties &setProperty(const Properties &name,
                                  const std::any &value);
  bool addProperty(const Properties &name, const std::any &value);
  bool removeProperty(const Properties &name);
  bool hasProperty(const Properties &name) const;
  template <typename T>
  std::optional<T> getPropertyAs(const Properties &name) const {
    if (hasProperty(name)) {
      try {
        return std::any_cast<T>(m_prop.at(name));
      } catch (const std::bad_any_cast &e) {
        std::cout << e.what() << std::endl;
      }
    }
    return std::nullopt;
  }

 private:
  std::map<Properties, std::any> m_prop;
};
