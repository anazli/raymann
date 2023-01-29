#pragma once

#include <any>
#include <map>

#include "tools/mat4.h"
#include "tools/vec3.h"

// objectTransformMatrix  Mat4f
// patternTransformMatrix Mat4f
// color         					Vec3f
// patternColorA 					Vec3f
// patternColorB 					Vec3f
// ambient      					float
// diffuse      					float
// specular     					float
// shininess    					float
// reflection   					float
// sphereCenter 					Point3f
// sphereRadius 					float

class Properties {
 public:
  Properties &setProperty(const std::string &name, const std::any &value);
  bool addProperty(const std::string &name, const std::any &value);
  bool removeProperty(const std::string &name);
  bool hasProperty(const std::string &name);
  std::any getPropertyAsAny(const std::string &name);
  int getPropertyAsInt(const std::string &name);
  float getPropertyAsFloat(const std::string &name);
  Vec3f getPropertyAsVec3f(const std::string &name);
  Mat4f getPropertyAsMat4f(const std::string &name);
  Point3f getPropertyAsPoint3f(const std::string &name);

 private:
  std::map<std::string, std::any> m_prop;
};
