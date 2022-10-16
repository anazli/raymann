#pragma once

#include "tools/mat4.h"
#include "tools/vec3.h"

class Properties {
 public:
  Properties() {}
  Properties &setObjTrans(const Mat4f &oTrans) {
    m_object_trans = oTrans;
    return *this;
  }
  Properties &setPatTrans(const Mat4f &pTrans) {
    m_pattern_trans = pTrans;
    return *this;
  }
  Properties &setColor(const Vec3f &color) {
    m_color = color;
    return *this;
  }
  Properties &setPatColA(const Vec3f &color) {
    m_pattern_colorA = color;
    return *this;
  }
  Properties &setPatColB(const Vec3f &color) {
    m_pattern_colorB = color;
    return *this;
  }
  Properties &setAmbient(const float &am) {
    m_ambient = am;
    return *this;
  }
  Properties &setDiffuse(const float &di) {
    m_diffuse = di;
    return *this;
  }
  Properties &setSpecular(const float &sp) {
    m_specular = sp;
    return *this;
  }
  Properties &setShininess(const float &sh) {
    m_shininess = sh;
    return *this;
  }
  Properties &setReflection(const float &ref) {
    m_reflective = ref;
    return *this;
  }

  Mat4f objTrans() const { return m_object_trans; }
  Mat4f patTrans() const { return m_pattern_trans; }
  Vec3f color() const { return m_color; }
  Vec3f patColA() const { return m_pattern_colorA; }
  Vec3f patColB() const { return m_pattern_colorB; }
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
