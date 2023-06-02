#pragma once

#include "composite/scene_element.h"
#include "visitors/renderer.h"

class Plane : public SceneElement {
 public:
  Plane() = default;
  bool intersect(const Ray &r) override {
    if (fabs(r.direction().y()) < EPS) {
      return false;
    }
    m_rec.t1 = -r.origin().y() / r.direction().y();
    m_rec.count = 1;
    return true;
  }
  Vec3f normal(const Point3f &p) const override {
    return Vec3f(0.0f, 1.0f, 0.0f);
  }
};
