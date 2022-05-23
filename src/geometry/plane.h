#pragma once

#include "geometry/traceable.h"

class Plane : public Traceable {
 public:
  Plane() = default;
  virtual ~Plane() {}
  bool isWorld() const override { return false; }
  bool intersect(const Ray &r) override {
    if (fabs(r.direction().y()) < EPS) {
      return false;
    }
    rec.t1 = -r.origin().y() / r.direction().y();
    rec.count = 1;
    return true;
  }
  Vec3f normal(const Point3f &p) const override {
    return Vec3f(0.0f, 1.0f, 0.0f);
  }
  Record record() const override { return rec; }
  bool isShadowed(const PointLight &l, const Point3f &p) override {
    Vec3f v = p - l.position();
    float distance = v.length();
    Ray r(l.position(), v.normalize());
    if (intersect(r) && record().t_min() >= 0.0f && record().t_min() < distance)
      return true;
    return false;
  }
};
