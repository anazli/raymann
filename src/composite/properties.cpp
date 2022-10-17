#include "composite/properties.h"

Properties::Properties() {}

Properties& Properties::setObjTrans(const Mat4f& oTrans) {
  m_object_trans = oTrans;
  return *this;
}

Properties& Properties::setPatTrans(const Mat4f& pTrans) {
  m_pattern_trans = pTrans;
  return *this;
}

Properties& Properties::setColor(const Vec3f& color) {
  m_color = color;
  return *this;
}

Properties& Properties::setPatColA(const Vec3f& color) {
  m_pattern_colorA = color;
  return *this;
}

Properties& Properties::setPatColB(const Vec3f& color) {
  m_pattern_colorB = color;
  return *this;
}

Properties& Properties::setAmbient(const float& am) {
  m_ambient = am;
  return *this;
}

Properties& Properties::setDiffuse(const float& di) {
  m_diffuse = di;
  return *this;
}

Properties& Properties::setSpecular(const float& sp) {
  m_specular = sp;
  return *this;
}

Properties& Properties::setShininess(const float& sh) {
  m_shininess = sh;
  return *this;
}

Properties& Properties::setReflection(const float& ref) {
  m_reflective = ref;
  return *this;
}

Properties& Properties::setSphereCenter(const Point3f& c) {
  m_sphere_center = c;
  return *this;
}

Properties& Properties::setSphereRadius(const float& r) {
  m_sphere_radius = r;
  return *this;
}

Mat4f Properties::objTrans() const { return m_object_trans; }

Mat4f Properties::patTrans() const { return m_pattern_trans; }

Vec3f Properties::color() const { return m_color; }

Vec3f Properties::patColA() const { return m_pattern_colorA; }

Vec3f Properties::patColB() const { return m_pattern_colorB; }

float Properties::ambient() const { return m_ambient; }

float Properties::diffuse() const { return m_diffuse; }

float Properties::specular() const { return m_specular; }

float Properties::shininess() const { return m_shininess; }

float Properties::reflection() const { return m_reflective; }

Point3f Properties::sphereCenter() const { return m_sphere_center; }

float Properties::sphereRadius() const { return m_sphere_radius; }
