#pragma once

#include <limits>

#include "geometry/primitive.h"

class Cone : public Primitive {
 public:
  Cone(const Transformation &tr = Transformation(),
       float minY = -std::numeric_limits<float>::max(),
       float maxY = std::numeric_limits<float>::max(), bool closed = false)
      : m_minimumY(minY), m_maximumY(maxY), m_closed(closed), Primitive(tr) {
    if (!closed) {
      m_object_box.minPoint() =
          Point3f(-limit::infinity(), -limit::infinity(), -limit::infinity());
      m_object_box.maxPoint() =
          Point3f(limit::infinity(), limit::infinity(), limit::infinity());
    } else {
      auto a = fabs(m_minimumY);
      auto b = fabs(m_maximumY);
      auto lim = std::max(a, b);
      m_object_box.minPoint() = Point3f(-lim, m_minimumY, -lim);
      m_object_box.maxPoint() = Point3f(lim, m_maximumY, lim);
    }
    m_world_box = m_transformation.objectToWorldSpace(m_object_box);
  }
  ~Cone() override = default;

  float minimumY() const { return m_minimumY; }
  float maximumY() const { return m_maximumY; }

  bool intersect(const Ray &r, Intersection &record) override {
    auto transf_ray = m_transformation.worldToObjectSpace(r);
    auto origin = transf_ray.origin();
    auto direction = transf_ray.direction();
    auto rdx = direction.x();
    auto rox = origin.x();
    auto rdy = direction.y();
    auto roy = origin.y();
    auto rdz = direction.z();
    auto roz = origin.z();

    auto a = rdx * rdx - rdy * rdy + rdz * rdz;
    auto b = 2.0f * (rox * rdx - roy * rdy + roz * rdz);
    auto c = rox * rox - roy * roy + roz * roz;
    float t1{}, t2{};
    if ((a <= EPS && a >= -EPS) && (b >= EPS || b <= -EPS)) {
      t1 = -c / (2.f * b);
      return true;
    }

    auto discr = b * b - 4.0f * a * c;
    auto hitAnything = false;
    if (discr >= 0.0f) {
      t1 = (-b - sqrt(discr)) / (2.0f * a);
      t2 = (-b + sqrt(discr)) / (2.0f * a);
      if (t1 > t2) std::swap(t1, t2);

      auto y1 = origin.y() + t1 * direction.y();
      if (m_minimumY < y1 && m_maximumY > y1) {
        hitAnything = true;
      }

      auto y2 = origin.y() + t2 * direction.y();
      if (m_minimumY < y2 && m_maximumY > y2) {
        hitAnything = true;
      }
    }
    if (hitAnything) {
      record.thit = Intersection::getMinHitParam(transf_ray, {t1, t2});
      record.hit_point = record.getHitPoint(transf_ray);
      record.normal = normal(record.hit_point);
    }
    if (intersectCaps(transf_ray, record)) hitAnything = true;
    return hitAnything;
  }
  Normal3f normal(const Point3f &p) const override {
    auto object_point = m_transformation.worldToObjectSpace(p);
    auto distance = object_point.x() * object_point.x() +
                    object_point.z() * object_point.z();
    if (distance < 1.f && object_point.y() >= m_maximumY - EPS) {
      return Normal3f(0.f, 1.f, 0.f);
    } else if (distance < 1.f && object_point.y() <= m_minimumY + EPS) {
      return Normal3f(0.f, -1.f, 0.f);
    }
    auto object_normal = Normal3f(object_point.x(), 0.f, object_point.z());
    return getUnitVectorOf(m_transformation.objectToWorldSpace(object_normal));
  }

  bool isClosed() const { return m_closed; }

  static PrimitivePtr create(const Transformation &tr = Transformation(),
                             float minY = -std::numeric_limits<float>::max(),
                             float maxY = std::numeric_limits<float>::max(),
                             bool closed = false) {
    return std::make_shared<Cone>(tr, minY, maxY, closed);
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

  bool intersectCaps(const Ray &r, Intersection &record) {
    if (!m_closed || (r.direction().y() <= EPS && r.direction().y() >= -EPS))
      return false;

    auto intersectsCap = false;
    auto t = (m_minimumY - r.origin().y()) / r.direction().y();
    float t1{}, t2{};
    if (checkCap(r, t, m_minimumY)) {
      t1 = t;
      intersectsCap = true;
    }
    t = (m_maximumY - r.origin().y()) / r.direction().y();
    if (checkCap(r, t, m_maximumY)) {
      t2 = t;
      intersectsCap = true;
    }
    record.thit = Intersection::getMinHitParam(r, {t1, t2});
    return intersectsCap;
  }
};
