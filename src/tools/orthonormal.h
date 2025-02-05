#pragma once

#include "tools/vec3.h"

class OrthoNormalBasis {
 public:
  OrthoNormalBasis() = default;
  Vec3D u() const { return m_u; }
  Vec3D v() const { return m_v; }
  Vec3D w() const { return m_w; }

  Vec3D local(float a, float b, float c) const {
    return a * m_u + b * m_v + c * m_w;
  }

  Vec3D local(const Vec3D& a) const {
    return a.x() * m_u + a.y() * m_v + a.z() * m_w;
  }

  void buildFromW(const Vec3D& w) {
    auto unit_w = getUnitVectorOf(w);
    auto a =
        (fabs(unit_w.x()) > 0.9f) ? Vec3D(0.f, 1.f, 0.f) : Vec3D(1.f, 0.f, 0.f);
    auto v = getUnitVectorOf(cross(unit_w, a));
    auto u = cross(unit_w, v);
    m_u = u;
    m_v = v;
    m_w = unit_w;
  }

 private:
  Vec3D m_u;
  Vec3D m_v;
  Vec3D m_w;
};
