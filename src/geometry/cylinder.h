#pragma once

#include "composite/scene_element.h"

class Cylinder : public SceneElement {
 public:
  ~Cylinder() override = default;

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
      record.t1 = (-b - sqrt(discr)) / (2.0f * a);
      record.t2 = (-b + sqrt(discr)) / (2.0f * a);
      record.count = 2;
      return true;
    }
    return false;
  }
  Vec3f normal(const Point3f &p) const override {
    return Vec3f(p.x(), 0.f, p.z());
  }

 private:
};
