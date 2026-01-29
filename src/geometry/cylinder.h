#pragma once

#include <limits>

#include "geometry/primitive.h"

class Cylinder : public Primitive {
 public:
  Cylinder(float minY = -std::numeric_limits<float>::max(),
           float maxY = std::numeric_limits<float>::max(), bool closed = false,
           const Transformation &tr = Transformation())
      : m_minimumY(minY), m_maximumY(maxY), m_closed(closed), Primitive(tr) {
    if (!closed) {
      m_object_box.minPoint() = Point3f(-1.f, -limit::infinity(), -1.f);
      m_object_box.maxPoint() = Point3f(1.f, limit::infinity(), 1.f);
    } else {
      m_object_box.minPoint() = Point3f(-1.f, m_minimumY, -1.f);
      m_object_box.maxPoint() = Point3f(1.f, m_maximumY, 1.f);
    }
    m_world_box = m_transformation.objectToWorldSpace(m_object_box);
  }
  ~Cylinder() override = default;

  float minimumY() const { return m_minimumY; }
  float maximumY() const { return m_maximumY; }

  bool intersect(const Ray &r, Intersection &record) override {
    auto transf_ray = m_transformation.worldToObjectSpace(r);
    auto origin = transf_ray.origin();
    auto direction = transf_ray.direction();
    auto rdx = direction.x();
    auto rox = origin.x();
    auto rdz = direction.z();
    auto roz = origin.z();

    auto a = rdx * rdx + rdz * rdz;
    if (a <= EPS && a >= -EPS) return intersectCaps(transf_ray, record);
    auto b = 2.0f * (rox * rdx + roz * rdz);
    auto c = rox * rox + roz * roz - 1.0f;

    auto discr = b * b - 4.0f * a * c;
    auto hitAnything = false;
    if (discr >= 0.0f) {
      auto t1 = static_cast<float>(-b - sqrt(discr)) / (2.0f * a);
      auto t2 = static_cast<float>(-b + sqrt(discr)) / (2.0f * a);
      if (t1 > t2) std::swap(t1, t2);

      auto y1 = origin.y() + t1 * direction.y();
      if (m_minimumY < y1 && m_maximumY > y1) {
        hitAnything = true;
      }

      auto y2 = origin.y() + t2 * direction.y();
      if (m_minimumY < y2 && m_maximumY > y2) {
        hitAnything = true;
      }

      if (hitAnything) {
        record.thit = Intersection::getMinHitParam(transf_ray, {t1, t2});
        record.hit_point = record.getHitPoint(transf_ray);
        record.normal = normal(record.hit_point);
      }
    }
    if (intersectCaps(transf_ray, record)) hitAnything = true;
    return hitAnything;
  }
  Normal3f normal(const Point3f &p) const override {
    auto object_point = m_transformation.worldToObjectSpace(p);
    auto distance = object_point.x() * object_point.x() +
                    object_point.z() * object_point.z();
    Normal3f object_normal;
    if (distance < 1.f && object_point.y() >= m_maximumY - EPS) {
      object_normal = Normal3f(0.f, 1.f, 0.f);
    } else if (distance < 1.f && object_point.y() <= m_minimumY + EPS) {
      object_normal = Normal3f(0.f, -1.f, 0.f);
    } else {
      object_normal = Normal3f(object_point.x(), 0.f, object_point.z());
    }
    return normalized(m_transformation.objectToWorldSpace(object_normal));
  }

  bool isClosed() const { return m_closed; }

  static PrimitivePtr create(float minY = -std::numeric_limits<float>::max(),
                             float maxY = std::numeric_limits<float>::max(),
                             bool closed = false,
                             const Transformation &tr = Transformation()) {
    return std::make_shared<Cylinder>(minY, maxY, closed, tr);
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
      intersectsCap = true;
    }
    t = (m_maximumY - r.origin().y()) / r.direction().y();
    if (checkCap(r, t)) {
      intersectsCap = true;
    }

    if (intersectsCap) {
      record.thit = t;
      record.hit_point = record.getHitPoint(r);
      record.normal = normal(record.hit_point);
    }
    return intersectsCap;
  }
};
