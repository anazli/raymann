#pragma once

#include <limits>

#include "composite/scene_element.h"

class Cone : public SceneElement {
 public:
  Cone(float minY = -std::numeric_limits<float>::max(),
       float maxY = std::numeric_limits<float>::max(), bool closed = false)
      : m_minimumY(minY), m_maximumY(maxY), m_closed(closed) {
    if (!closed) {
      m_bBox.minPoint() =
          Point3D(-limit::infinity(), -limit::infinity(), -limit::infinity());
      m_bBox.maxPoint() =
          Point3D(limit::infinity(), limit::infinity(), limit::infinity());
    } else {
      auto a = fabs(m_minimumY);
      auto b = fabs(m_maximumY);
      auto lim = std::max(a, b);
      m_bBox.minPoint() = Point3D(-lim, m_minimumY, -lim);
      m_bBox.maxPoint() = Point3D(lim, m_maximumY, lim);
    }
  }
  ~Cone() override = default;

  float minimumY() const { return m_minimumY; }
  float maximumY() const { return m_maximumY; }

  bool intersect(const Ray &r, IntersectionRecord &record) override {
    auto transformed_ray = r.transform(m_transformation.getInverseMatrix());
    auto origin = transformed_ray.origin();
    auto direction = transformed_ray.direction();
    auto rdx = direction.x();
    auto rox = origin.x();
    auto rdy = direction.y();
    auto roy = origin.y();
    auto rdz = direction.z();
    auto roz = origin.z();

    auto a = rdx * rdx - rdy * rdy + rdz * rdz;
    auto b = 2.0f * (rox * rdx - roy * rdy + roz * rdz);
    auto c = rox * rox - roy * roy + roz * roz;
    if ((a <= EPS && a >= -EPS) && (b >= EPS || b <= -EPS)) {
      record.t1 = -c / (2.f * b);
      record.count++;
      return true;
    }

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
    if (intersectCaps(transformed_ray, record)) hitAnything = true;
    return hitAnything;
  }
  Vec3D normal(const Point3D &p) const override {
    auto v4 = Vec4D(p);
    auto object_point = m_transformation.getInverseMatrix() * v4;
    auto distance = object_point.x() * object_point.x() +
                    object_point.z() * object_point.z();
    if (distance < 1.f && object_point.y() >= m_maximumY - EPS) {
      return Vec3D(0.f, 1.f, 0.f);
    } else if (distance < 1.f && object_point.y() <= m_minimumY + EPS) {
      return Vec3D(0.f, -1.f, 0.f);
    }
    auto object_normal = Vec3D(object_point.x(), 0.f, object_point.z());
    return Vec3D(getUnitVectorOf(m_transformation.getInverseTransposeMatrix() *
                                 Vec4D(object_normal)));
  }

  bool isClosed() const { return m_closed; }

  static SceneElementPtr create(float minY = -std::numeric_limits<float>::max(),
                                float maxY = std::numeric_limits<float>::max(),
                                bool closed = false) {
    return std::make_shared<Cone>(minY, maxY, closed);
  }

 private:
  float m_minimumY;
  float m_maximumY;
  bool m_closed;

  bool checkCap(const Ray &r, float t, float y) {
    auto x = r.origin().x() + t * r.direction().x();
    auto z = r.origin().z() + t * r.direction().z();
    return x * x + z * z <= fabs(y);
  }

  bool intersectCaps(const Ray &r, IntersectionRecord &record) {
    if (!m_closed || (r.direction().y() <= EPS && r.direction().y() >= -EPS))
      return false;

    auto intersectsCap = false;
    auto t = (m_minimumY - r.origin().y()) / r.direction().y();
    if (checkCap(r, t, m_minimumY)) {
      record.t1 = t;
      record.count++;
      intersectsCap = true;
    }
    t = (m_maximumY - r.origin().y()) / r.direction().y();
    if (checkCap(r, t, m_maximumY)) {
      record.t2 = t;
      record.count++;
      intersectsCap = true;
    }
    return intersectsCap;
  }
};
