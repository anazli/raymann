#pragma once

#include "composite/scene_element.h"

class Cylinder : public SceneElement {
 public:
  Cylinder(const float minY = -MAXFLOAT, const float maxY = MAXFLOAT)
      : m_minimumY(minY), m_maximumY(maxY) {}
  ~Cylinder() override = default;

  float minimumY() const { return m_minimumY; }
  float maximumY() const { return m_maximumY; }

  bool intersect(const Ray &r, IntersectionRecord &record) override {
    auto rdx = r.direction().x();
    auto rox = r.origin().x();
    auto rdz = r.direction().z();
    auto roz = r.origin().z();

    auto a = rdx * rdx + rdz * rdz;
    if (a <= EPS) return false;
    auto b = 2.0f * (rox * rdx + roz * rdz);
    auto c = rox * rox + roz * roz - 1.0f;

    auto discr = b * b - 4.0f * a * c;
    if (discr >= 0.0f) {
      auto t1 = (-b - sqrt(discr)) / (2.0f * a);
      auto t2 = (-b + sqrt(discr)) / (2.0f * a);
      if (t1 > t2) std::swap(t1, t2);

      auto y1 = r.origin().y() + t1 * r.direction().y();
      if (m_minimumY < y1 && m_maximumY > y1) {
        record.t1 = t1;
        record.count++;
      }

      auto y2 = r.origin().y() + t2 * r.direction().y();
      if (m_minimumY < y2 && m_maximumY > y2) {
        record.t2 = t2;
        record.count++;
      }
      return true;
    }
    return false;
  }
  Vec3f normal(const Point3f &p) const override {
    return Vec3f(p.x(), 0.f, p.z());
  }

 private:
  float m_minimumY;
  float m_maximumY;
};
