#pragma once

#include "composite/scene_element.h"

class Sphere : public SceneElement {
 public:
  ~Sphere() override = default;
  Sphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f), const float &r = 1.0f)
      : m_center(c), m_radius(r) {
    setParent(nullptr);
  }

  bool intersect(const Ray &r, IntersectionRecord &record) override;
  Vec3f normal(const Point3f &p) const override {
    return (p - m_center).normalize();
  }
  void setCenter(const Point3f &c) { m_center = c; }
  void setRadius(const float &r) { m_radius = r; }
  Point3f center() const { return m_center; }
  float radius() const { return m_radius; }

 private:
  Point3f m_center;
  float m_radius;
};

inline bool Sphere::intersect(const Ray &r, IntersectionRecord &record) {
  auto co = r.origin() - m_center;
  auto a = dot(r.direction(), r.direction());
  auto b = 2.0f * dot(r.direction(), co);
  auto c = dot(co, co) - m_radius * m_radius;
  auto discr = b * b - 4.0f * a * c;
  if (discr >= 0.0f) {
    auto t1 = (-b - sqrt(discr)) / (2. * a);
    auto t2 = (-b + sqrt(discr)) / (2. * a);
    record.t1 = t1;
    record.t2 = t2;
    record.count = 2;
    return true;
  }
  return false;
}
