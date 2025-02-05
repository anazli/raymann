#pragma once

#include <any>
#include <map>

#include "tools/tools.h"

enum class Props {
  COLOR,            // Vec3D
  AMBIENT,          // float [0,1]
  DIFFUSE,          // float [0,1]
  SPECULAR,         // float [0,1]
  SHININESS,        // float [0,..]
  REFLECTION,       // float [0,1]
  TRANSPARENCY,     // float [0,1]
  REFRACTIVE_INDEX  // float [0,..]
};

class MaterialProperties {
 public:
  MaterialProperties();
  MaterialProperties &setProperty(const Props &name, const std::any &value);
  bool addProperty(const Props &name, const std::any &value);
  bool removeProperty(const Props &name);
  bool hasProperty(const Props &name) const;
  std::any getPropertyAsAny(const Props &name) const;
  int getPropertyAsInt(const Props &name) const;
  float getPropertyAsFloat(const Props &name) const;
  Vec3D getPropertyAsVec3D(const Props &name) const;
  Mat4D getPropertyAsMat4D(const Props &name) const;
  Point3D getPropertyAsPoint3D(const Props &name) const;

 private:
  std::map<Props, std::any> m_prop;
};
