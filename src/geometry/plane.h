#pragma once

#include "composite/scene_element.h"

class Plane : public SceneElement {
 public:
  Plane() {
    m_elementType = SceneElementType::PLANE;
    m_bBox.minPoint() = Point3D(-limit::infinity(), 0.f, -limit::infinity());
    m_bBox.maxPoint() = Point3D(limit::infinity(), 0.f, limit::infinity());
  }
  ~Plane() override = default;
  bool intersect(const Ray &r, IntersectionRecord &record) override {
    auto transformed_ray = r.transform(m_transformation.getInverseMatrix());
    auto origin = transformed_ray.origin();
    auto direction = transformed_ray.direction();
    if (fabs(direction.y()) < EPS) {
      return false;
    }
    record.t1 = -origin.y() / direction.y();
    record.count = 1;
    record.saved_point = record.point(transformed_ray);
    return true;
  }
  Vec3D normal(const Point3D &p) const override {
    return Vec3D(0.0f, 1.0f, 0.0f);
  }
};
