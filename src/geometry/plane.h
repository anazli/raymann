#pragma once

#include "geometry/primitive.h"

class Plane : public Primitive {
 public:
  Plane(const Transformation &tr = Transformation()) : Primitive(tr) {
    m_object_box.minPoint() =
        Point3f(-limit::infinity(), 0.f, -limit::infinity());
    m_object_box.maxPoint() =
        Point3f(limit::infinity(), 0.f, limit::infinity());
    m_world_box = m_transformation.objectToWorldSpace(m_object_box);
  }
  ~Plane() override = default;
  bool intersect(const Ray &r, Intersection &record) override {
    auto transf_ray = m_transformation.worldToObjectSpace(r);
    auto origin = transf_ray.origin();
    auto direction = transf_ray.direction();
    if (fabs(direction.y()) < EPS) {
      return false;
    }
    record.thit = -origin.y() / direction.y();
    record.hit_point = record.getHitPoint(transf_ray);
    record.normal = normal(record.hit_point);
    return true;
  }
  Normal3f normal(const Point3f &p) const override {
    return Normal3f(0.0f, 1.0f, 0.0f);
  }

  static PrimitivePtr create(const Transformation &tr = Transformation()) {
    return std::make_shared<Plane>(tr);
  }
};
