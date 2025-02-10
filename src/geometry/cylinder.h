#pragma once

#include <limits>

#include "composite/scene_element.h"

class Cylinder : public SceneElement {
 public:
  Cylinder(float minY = -std::numeric_limits<float>::max(),
           float maxY = std::numeric_limits<float>::max(), bool closed = false)
      : m_minimumY(minY), m_maximumY(maxY), m_closed(closed) {
    if (!closed) {
      m_bBox.minPoint() = Point3D(-1.f, -limit::infinity(), -1.f);
      m_bBox.maxPoint() = Point3D(1.f, limit::infinity(), 1.f);
    } else {
      m_bBox.minPoint() = Point3D(-1.f, m_minimumY, -1.f);
      m_bBox.maxPoint() = Point3D(1.f, m_maximumY, 1.f);
    }
  }
  ~Cylinder() override = default;

  float minimumY() const { return m_minimumY; }
  float maximumY() const { return m_maximumY; }

  bool intersect(const Ray &r, Intersection &record) override {
    auto transformed_ray = r.transform(m_transformation.getInverseMatrix());
    auto origin = transformed_ray.origin();
    auto direction = transformed_ray.direction();
    auto rdx = direction.x();
    auto rox = origin.x();
    auto rdz = direction.z();
    auto roz = origin.z();

    auto a = rdx * rdx + rdz * rdz;
    if (a <= EPS && a >= -EPS) return intersectCaps(transformed_ray, record);
    auto b = 2.0f * (rox * rdx + roz * rdz);
    auto c = rox * rox + roz * roz - 1.0f;

    auto discr = b * b - 4.0f * a * c;
    auto hitAnything = false;
    if (discr >= 0.0f) {
      auto t1 = (-b - sqrt(discr)) / (2.0f * a);
      auto t2 = (-b + sqrt(discr)) / (2.0f * a);
      if (t1 > t2) std::swap(t1, t2);

      auto y1 = origin.y() + t1 * direction.y();
      if (m_minimumY < y1 && m_maximumY > y1) {
        record.t1 = t1;
        record.count++;
        hitAnything = true;
      }

      auto y2 = origin.y() + t2 * direction.y();
      if (m_minimumY < y2 && m_maximumY > y2) {
        record.t2 = t2;
        record.count++;
        hitAnything = true;
      }
    }
    record.saved_point = record.point(transformed_ray);
    if (intersectCaps(transformed_ray, record)) hitAnything = true;
    return hitAnything;
  }
  Vec3D normal(const Point3D &p) const override {
    auto v4 = Vec4D(p);
    auto object_point = m_transformation.getInverseMatrix() * v4;
    auto distance = object_point.x() * object_point.x() +
                    object_point.z() * object_point.z();
    Vec3D object_normal;
    if (distance < 1.f && object_point.y() >= m_maximumY - EPS) {
      object_normal = Vec3D(0.f, 1.f, 0.f);
    } else if (distance < 1.f && object_point.y() <= m_minimumY + EPS) {
      object_normal = Vec3D(0.f, -1.f, 0.f);
    } else
      object_normal = Vec3D(object_point.x(), 0.f, object_point.z());
    auto world_normal =
        m_transformation.getInverseTransposeMatrix() * Vec4D(object_normal);
    return Vec3D(getUnitVectorOf(world_normal));
  }

  bool isClosed() const { return m_closed; }

  static SceneElementPtr create(float minY = -std::numeric_limits<float>::max(),
                                float maxY = std::numeric_limits<float>::max(),
                                bool closed = false) {
    return std::make_shared<Cylinder>(minY, maxY, closed);
  }

 private:
  float m_minimumY;
  float m_maximumY;
  bool m_closed;

  bool checkCap(const Ray &r, float t) {
    auto x = r.origin().x() + t * r.direction().x();
    auto z = r.origin().z() + t * r.direction().z();
    return x * x + z * z <= 1.f;
  }

  bool intersectCaps(const Ray &r, Intersection &record) {
    if (!m_closed || (r.direction().y() <= EPS && r.direction().y() >= -EPS))
      return false;

    auto intersectsCap = false;
    auto t = (m_minimumY - r.origin().y()) / r.direction().y();
    if (checkCap(r, t)) {
      record.t1 = t;
      record.count++;
      intersectsCap = true;
    }
    t = (m_maximumY - r.origin().y()) / r.direction().y();
    if (checkCap(r, t)) {
      record.t2 = t;
      record.count++;
      intersectsCap = true;
    }
    return intersectsCap;
  }
};
