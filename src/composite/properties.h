#pragma once

#include <any>
#include <map>

#include "tools/mat4.h"
#include "tools/vec3.h"

class Properties {
 public:
  Properties();
  Properties &setObjTrans(const Mat4f &oTrans);
  Properties &setPatTrans(const Mat4f &pTrans);
  Properties &setColor(const Vec3f &color);
  Properties &setPatColA(const Vec3f &color);
  Properties &setPatColB(const Vec3f &color);
  Properties &setAmbient(const float &am);
  Properties &setDiffuse(const float &di);
  Properties &setSpecular(const float &sp);
  Properties &setShininess(const float &sh);
  Properties &setReflection(const float &ref);
  Properties &setSphereCenter(const Point3f &c);
  Properties &setSphereRadius(const float &r);

  Mat4f objTrans() const;
  Mat4f patTrans() const;
  Vec3f color() const;
  Vec3f patColA() const;
  Vec3f patColB() const;
  float ambient() const;
  float diffuse() const;
  float specular() const;
  float shininess() const;
  float reflection() const;
  Point3f sphereCenter() const;
  float sphereRadius() const;

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
  Point3f m_sphere_center = Point3f(0.f, 0.f, 0.f);
  float m_sphere_radius = 1.f;
  std::map<std::string, std::any> m_prop;
};
