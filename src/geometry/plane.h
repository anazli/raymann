#pragma once

#include "composite/scene_element.h"
#include "renderers/renderer.h"

class Plane : public SceneElement {
 public:
  Plane(){};
  bool intersect(const Ray &r, IntersectionRecord &record) override {
    if (fabs(r.direction().y()) < EPS) {
      return false;
    }
    record.t1 = -r.origin().y() / r.direction().y();
    record.count = 1;
    return true;
  }
  Vec3f normal(const Point3f &p) const override {
    return Vec3f(0.0f, 1.0f, 0.0f);
  }
};
