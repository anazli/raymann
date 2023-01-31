#pragma once

#include <any>
#include <map>

#include "tools/mat4.h"
#include "tools/vec3.h"

enum class Props {
  COLOR = 10,                      // Vec3f
  AMBIENT = 20,                    // float [0,1]
  DIFFUSE = 30,                    // float [0,1]
  SPECULAR = 40,                   // float [0,1]
  SHININESS = 50,                  // float [0,..]
  REFLECTION = 60,                 // float [0,1]
  PATTERN_COLORA = 70,             // Vec3f
  PATTERN_COLORB = 80,             // Vec3f
  OBJECT_TRANSFROM_MATRIX = 90,    // Mat4f
  PATTERN_TRANSFORM_MATRIX = 100,  // Mat4f
  SPHERE_CENTER = 110,             // Point3f
  SPHERE_RADIUS = 120              // float
};

class Properties {
 public:
  Properties();
  Properties &setProperty(const Props &name, const std::any &value);
  bool addProperty(const Props &name, const std::any &value);
  bool removeProperty(const Props &name);
  bool hasProperty(const Props &name) const;
  std::any getPropertyAsAny(const Props &name) const;
  int getPropertyAsInt(const Props &name) const;
  float getPropertyAsFloat(const Props &name) const;
  Vec3f getPropertyAsVec3f(const Props &name) const;
  Mat4f getPropertyAsMat4f(const Props &name) const;
  Point3f getPropertyAsPoint3f(const Props &name) const;

 private:
  std::map<Props, std::any> m_prop;
};
