#pragma once

#include "tools/tools.h"

class Ray {
 public:
  Ray() {}
  Ray(const Point3f &ori, const Vec3f &dir) : m_ori(ori), m_dir(dir) {}

  void setOrigin(const Point3f &ori) { m_ori = ori; }
  void setDirection(const Vec3f &dir) { m_dir = dir; }
  Point3f origin() const { return m_ori; }
  Vec3f direction() const { return m_dir; }
  Point3f position(const float &t) const { return origin() + t * direction(); }
  Ray transform(const Mat4f &m) const {
    Ray ret;

    Vec4f v4(origin());
    v4 = m * v4;
    Point3f new_o = v4;
    ret.setOrigin(new_o);

    v4 = direction();
    v4 = m * v4;
    Vec4f new_d = v4;
    ret.setDirection(new_d);
    return ret;
  }

 private:
  Point3f m_ori;
  Vec3f m_dir;
};
