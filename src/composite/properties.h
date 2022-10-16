#pragma once

#include "tools/mat4.h"
#include "tools/vec3.h"

class Properties {
 public:
  Properties() {}
  void setObjectTransformation(const Mat4f &oTrans) { m_object_trans = oTrans; }
  void setPatternTransformation(const Mat4f &pTrans) {
    m_pattern_trans = pTrans;
  }
  void setColor(const Vec3f &color) { m_color = color; }
  void setPatternColorA(const Vec3f &color) { m_pattern_colorA = color; }
  void setPatternColorB(const Vec3f &color) { m_pattern_colorB = color; }
  void setAmbient(const float &am) { m_ambient = am; }
  void setDiffuse(const float &di) { m_diffuse = di; }
  void setSpecular(const float &sp) { m_specular = sp; }
  void setShininess(const float &sh) { m_shininess = sh; }
  void setReflection(const float &ref) { m_reflective = ref; }

  Mat4f objectTransformation() const { return m_object_trans; }
  Mat4f patternTransformation() const { return m_pattern_trans; }
  Vec3f color() const { return m_color; }
  Vec3f patternColorA() const { return m_pattern_colorA; }
  Vec3f patternColorB() const { return m_pattern_colorB; }
  float ambient() const { return m_ambient; }
  float diffuse() const { return m_diffuse; }
  float specular() const { return m_specular; }
  float shininess() const { return m_shininess; }
  float reflection() const { return m_reflective; }

 private:
  Mat4f m_object_trans = Mat4f();
  Mat4f m_pattern_trans = Mat4f();
  Vec3f m_color = Vec3f();
  Vec3f m_pattern_colorA = Vec3f();
  Vec3f m_pattern_colorB = Vec3f();
  float m_ambient = 0.1f;
  float m_diffuse = 0.9f;
  float m_specular = 0.9f;
  float m_shininess = 200.f;
  float m_reflective = 0.f;
};
