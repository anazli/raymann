#pragma once

#include "tools/point3.h"
#include "tools/ray.h"
#include "tools/vec3.h"
#include "traceable.h"

class Sphere : public Traceable {
 public:
  Sphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f), const float &r = 0.0f)
      : m_center(c), m_radius(r) {
    setParent(nullptr);
  }

  virtual ~Sphere() {}
  bool intersect(const Ray &r) override;
  bool isWorld() const override { return false; }
  void setCenter(const Point3f &c) { m_center = c; }
  void setRadius(const float &r) { m_radius = r; }
  Point3f center() const { return m_center; }
  float radius() const { return m_radius; }

 private:
  Point3f m_center;
  float m_radius;
};

inline bool Sphere::intersect(const Ray &r) {
  Vec3f co = r.origin() - center();
  float a = dot(r.direction(), r.direction());
  float b = 2.0f * dot(r.direction(), co);
  float c = dot(co, co) - radius() * radius();
  float discr = b * b - 4.0f * a * c;
  if (discr >= 0.0f) {
    float t1 = (-b - sqrt(discr)) / (2. * a);
    float t2 = (-b + sqrt(discr)) / (2. * a);
    rec.t1 = t1;
    rec.t2 = t2;
    rec.count += 2;
    return true;
  }
  return false;
}
