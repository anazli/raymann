#pragma once

#include "tools/vec3.h"

class OrthoNormalBasis {
 public:
  Vec3f u() const { return m_u; }
  Vec3f v() const { return m_v; }
  Vec3f w() const { return m_w; }

  Vec3f local(float a, float b, float c) const {
    return a * m_u + b * m_v + c * m_w;
  }

  Vec3f local(const Vec3f& a) const {
    return a.x() * m_u + a.y() * m_v + a.z() * m_w;
  }

  void buildFromW(const Vec3f& w) {
    auto unit_w = Vec3f(w).normalize();
    auto a =
        (fabs(unit_w.x()) > 0.9f) ? Vec3f(0.f, 1.f, 0.f) : Vec3f(1.f, 0.f, 0.f);
    auto v = Vec3f(cross(unit_w, a)).normalize();
    auto u = cross(unit_w, v);
    m_u = u;
    m_v = v;
    m_w = unit_w;
  }

 private:
  Vec3f m_u;
  Vec3f m_v;
  Vec3f m_w;
};
