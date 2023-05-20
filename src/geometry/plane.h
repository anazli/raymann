#pragma once

#include "composite/element.h"

class Plane : public Element {
 public:
  Plane() = default;
  virtual ~Plane() {}
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
};
