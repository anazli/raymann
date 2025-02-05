#pragma once

#include "composite/scene_element.h"
#include "stochastic/random.h"

class Quad : public SceneElement {
 public:
  Quad(const Point3D& origin, const Vec3D& uAxis, const Vec3D& vAxis)
      : m_origin(origin), m_uAxis(uAxis), m_vAxis(vAxis) {
    auto n = cross(m_uAxis, m_vAxis);
    auto normalV = getUnitVectorOf(n);
    m_dParam = dot(normalV, Vec3D(m_origin));
    m_wParam = normalV / dot(n, n);

    m_area = n.length();
    m_bBox.minPoint() = m_origin;
    m_bBox.maxPoint() = m_origin + m_uAxis + m_vAxis;
  }

  bool intersect(const Ray& r, IntersectionRecord& record) override {
    auto normalV = normal(record.point(r));
    auto denom = dot(normalV, r.direction());
    auto dParam = dot(normalV, Vec3D(m_origin));

    if (fabs(denom) < EPS) return false;

    auto t = (dParam - dot(normalV, Vec3D(r.origin()))) / denom;
    auto intersection = r.position(t);
    auto planarHitptVector = intersection - m_origin;

    auto alpha = dot(m_wParam, cross(planarHitptVector, m_vAxis));
    auto beta = dot(m_wParam, cross(m_uAxis, planarHitptVector));
    if (!isWithin(alpha, beta, record)) return false;

    record.t1 = t;
    record.saved_point = record.point(r);
    return true;
  }

  Vec3D normal(const Point3D& p) const override {
    auto n = cross(m_uAxis, m_vAxis);
    n.normalize();
    return n;
  }

  float pdf(const Point3D& origin, const Vec3D& direction) override {
    IntersectionRecord rec;
    if (!intersect(Ray(origin, direction), rec)) return 1.f;

    auto distSquared =
        rec.t_min() * rec.t_min() * direction.length() * direction.length();
    auto cosine = fabs(dot(direction, rec.normal) / direction.length());

    return distSquared / (cosine * m_area);
  }

  Vec3D random(const Point3D& origin) override {
    return m_origin + (Random::randomNumber() * m_uAxis) +
           (Random::randomNumber() * m_vAxis) - origin;
  }

 private:
  bool isWithin(float a, float b, IntersectionRecord& record) const {
    if ((a < 0.f) || (a > 1.f) || (b < 0.f) || (b > 1.f)) return false;
    // record.u = a;
    // record.v = b;
    return true;
  }

  Point3D m_origin;
  Vec3D m_uAxis;
  Vec3D m_vAxis;
  float m_dParam;
  Vec3D m_wParam;
  float m_area;
};
