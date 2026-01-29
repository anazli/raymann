#pragma once

#include "geometry/primitive.h"
#include "stochastic/random.h"

class Quad : public Primitive {
 public:
  Quad(const Point3f& origin, const Vec3f& uAxis, const Vec3f& vAxis,
       const Transformation& tr = Transformation())
      : m_origin(origin), m_uAxis(uAxis), m_vAxis(vAxis), Primitive(tr) {
    auto n = cross(m_uAxis, m_vAxis);
    auto normalV = getUnitVectorOf(n);
    m_dParam = dot(normalV, Vec3f(m_origin));
    m_wParam = normalV / dot(n, n);

    m_area = n.length();
    m_object_box.minPoint() = m_origin;
    m_object_box.maxPoint() = m_origin + m_uAxis + m_vAxis;
    m_world_box = m_transformation.objectToWorldSpace(m_object_box);
  }

  bool intersect(const Ray& r, Intersection& record) override {
    auto transf_ray = m_transformation.worldToObjectSpace(r);
    auto origin = transf_ray.origin();
    auto direction = transf_ray.direction();
    auto normalV = normal(record.getHitPoint(transf_ray));
    auto denom = dot(normalV, direction);
    auto dParam = dot(normalV, Vec3f(m_origin));

    if (fabs(denom) < EPS) return false;

    auto t = (dParam - dot(normalV, Vec3f(origin))) / denom;
    auto intersection = transf_ray.position(t);
    auto planarHitptVector = intersection - m_origin;

    auto alpha = dot(m_wParam, cross(planarHitptVector, m_vAxis));
    auto beta = dot(m_wParam, cross(m_uAxis, planarHitptVector));
    if (!isWithin(alpha, beta, record)) return false;

    record.thit = t;
    record.hit_point = record.getHitPoint(transf_ray);
    record.normal = normal(record.hit_point);
    return true;
  }

  Normal3f normal(const Point3f& p) const override {
    auto n = cross(m_uAxis, m_vAxis);
    n.normalize();
    return Normal3f(n);
  }

  static PrimitivePtr create(const Point3f& origin, const Vec3f& uAxis,
                             const Vec3f& vAxis,
                             const Transformation& tr = Transformation()) {
    return std::make_shared<Quad>(origin, uAxis, vAxis, tr);
  }

 private:
  bool isWithin(float a, float b, Intersection& record) const {
    if ((a < 0.f) || (a > 1.f) || (b < 0.f) || (b > 1.f)) return false;
    // record.u = a;
    // record.v = b;
    return true;
  }

  Point3f m_origin;
  Vec3f m_uAxis;
  Vec3f m_vAxis;
  float m_dParam;
  Vec3f m_wParam;
  float m_area;
};
