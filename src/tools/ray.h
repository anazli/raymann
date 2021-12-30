#pragma once

#include "point3.h"
#include "vec3.h"

class Ray {
 public:
  Ray() {}
  Ray(const Point3f &ori, const Vec3f &dir) : m_ori(ori), m_dir(dir) {}

  void setOrigin(const Point3f &ori) { m_ori = ori; }
  void setDirection(const Vec3f &dir) { m_dir = dir; }
  Point3f origin() const { return m_ori; }
  Vec3f direction() const { return m_dir; }

 private:
  Point3f m_ori;
  Vec3f m_dir;
};
