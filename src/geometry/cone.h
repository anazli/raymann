#pragma once

#include <limits>

#include "composite/scene_element.h"

class Cone : public SceneElement {
 public:
  Cone(float minY = -std::numeric_limits<float>::max(),
       float maxY = std::numeric_limits<float>::max(), bool closed = false)
      : m_minimumY(minY), m_maximumY(maxY), m_closed(closed) {
    m_elementType = SceneElementType::CONE;
    if (!closed) {
      m_bBoxProps.minPoint() =
          Point3f(-limit::infinity(), -limit::infinity(), -limit::infinity());
      m_bBoxProps.maxPoint() =
          Point3f(limit::infinity(), limit::infinity(), limit::infinity());
    } else {
      float a = fabs(m_minimumY);
      float b = fabs(m_maximumY);
      float lim = std::max(a, b);
      m_bBoxProps.minPoint() = Point3f(-lim, m_minimumY, -lim);
      m_bBoxProps.maxPoint() = Point3f(lim, m_maximumY, lim);
    }
  }
  ~Cone() override = default;

  float minimumY() const { return m_minimumY; }
  float maximumY() const { return m_maximumY; }

  bool intersect(const Ray &r, IntersectionRecord &record) override {
    auto rdx = r.direction().x();
    auto rox = r.origin().x();
    auto rdy = r.direction().y();
    auto roy = r.origin().y();
    auto rdz = r.direction().z();
    auto roz = r.origin().z();

    auto a = rdx * rdx - rdy * rdy + rdz * rdz;
    auto b = 2.0f * (rox * rdx - roy * rdy + roz * rdz);
    auto c = rox * rox - roy * roy + roz * roz;
    if ((a <= EPS && a >= -EPS) && (b >= EPS || b <= -EPS)) {
      record.t1 = -c / (2.f * b);
      record.count++;
      return true;
    }

    auto discr = b * b - 4.0f * a * c;
    bool hitAnything = false;
    if (discr >= 0.0f) {
      auto t1 = (-b - sqrt(discr)) / (2.0f * a);
      auto t2 = (-b + sqrt(discr)) / (2.0f * a);
      if (t1 > t2) std::swap(t1, t2);

      auto y1 = r.origin().y() + t1 * r.direction().y();
      if (m_minimumY < y1 && m_maximumY > y1) {
        record.t1 = t1;
        record.count++;
        hitAnything = true;
      }

      auto y2 = r.origin().y() + t2 * r.direction().y();
      if (m_minimumY < y2 && m_maximumY > y2) {
        record.t2 = t2;
        record.count++;
        hitAnything = true;
      }
    }
    if (intersectCaps(r, record)) hitAnything = true;
    return hitAnything;
  }
  Vec3f normal(const Point3f &p) const override {
    auto distance = p.x() * p.x() + p.z() * p.z();
    if (distance < 1.f && p.y() >= m_maximumY - EPS) {
      return Vec3f(0.f, 1.f, 0.f);
    } else if (distance < 1.f && p.y() <= m_minimumY + EPS) {
      return Vec3f(0.f, -1.f, 0.f);
    }
    return Vec3f(p.x(), 0.f, p.z());
  }

  bool isClosed() const { return m_closed; }

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

    bool intersectsCap = false;
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
