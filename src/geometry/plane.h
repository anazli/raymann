#pragma once

#include "composite/scene_element.h"

class Plane : public SceneElement {
 public:
  Plane() {
    m_bBox.minPoint() = Point3D(-limit::infinity(), 0.f, -limit::infinity());
    m_bBox.maxPoint() = Point3D(limit::infinity(), 0.f, limit::infinity());
  }
  ~Plane() override = default;
  bool intersect(const Ray &r, Intersection &record) override {
    auto transformed_ray = m_transformation.worldToObjectSpace(r);
    auto origin = transformed_ray.origin();
    auto direction = transformed_ray.direction();
    if (fabs(direction.y()) < EPS) {
      return false;
    }
    record.min_hit = -origin.y() / direction.y();
    record.hit_point = record.getHitPoint(transformed_ray);
    return true;
  }
  Normal3D normal(const Point3D &p) const override {
    return Normal3D(0.0f, 1.0f, 0.0f);
  }

  static SceneElementPtr create() { return std::make_shared<Plane>(); }
};
