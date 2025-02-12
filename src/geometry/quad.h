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

  bool intersect(const Ray& r, Intersection& record) override {
    auto transformed_ray = m_transformation.worldToObjectSpace(r);
    auto origin = transformed_ray.origin();
    auto direction = transformed_ray.direction();
    auto normalV = normal(record.point(transformed_ray));
    auto denom = dot(normalV, direction);
    auto dParam = dot(normalV, Vec3D(m_origin));

    if (fabs(denom) < EPS) return false;

    auto t = (dParam - dot(normalV, Vec3D(origin))) / denom;
    auto intersection = transformed_ray.position(t);
    auto planarHitptVector = intersection - m_origin;

    auto alpha = dot(m_wParam, cross(planarHitptVector, m_vAxis));
    auto beta = dot(m_wParam, cross(m_uAxis, planarHitptVector));
    if (!isWithin(alpha, beta, record)) return false;

    record.min_hit = t;
    record.saved_point = record.point(transformed_ray);
    return true;
  }

  Normal3D normal(const Point3D& p) const override {
    auto n = cross(m_uAxis, m_vAxis);
    n.normalize();
    return Normal3D(n);
  }

  float pdf(const Point3D& origin, const Vec3D& direction) override {
    Intersection rec;
    if (!intersect(Ray(origin, direction), rec)) return 1.f;

    auto distSquared =
        rec.min_hit * rec.min_hit * direction.length() * direction.length();
    auto cosine = fabs(dot(direction, rec.surface_normal) / direction.length());

    return distSquared / (cosine * m_area);
  }

  Vec3D random(const Point3D& origin) override {
    return m_origin + (Random::randomNumber() * m_uAxis) +
           (Random::randomNumber() * m_vAxis) - origin;
  }

  static SceneElementPtr create(const Point3D& origin, const Vec3D& uAxis,
                                const Vec3D& vAxis) {
    return std::make_shared<Quad>(origin, uAxis, vAxis);
  }

 private:
  bool isWithin(float a, float b, Intersection& record) const {
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
