#pragma once

#include <limits>

#include "geometry/primitive.h"
#include "stochastic/random.h"

class Cube : public Primitive {
 public:
  Cube(const Transformation &tr = Transformation()) : Primitive(tr) {
    m_object_box.minPoint() = Point3D(-1.f, -1.f, -1.f);
    m_object_box.maxPoint() = Point3D(1.f, 1.f, 1.f);
    m_world_box = m_transformation.objectToWorldSpace(m_object_box);
  }
  ~Cube() override = default;

  bool intersect(const Ray &r, Intersection &record) override {
    auto transf_ray = m_transformation.worldToObjectSpace(r);
    auto origin = transf_ray.origin();
    auto direction = transf_ray.direction();
    auto xMinMax = hitAxis(origin.x(), direction.x());
    auto yMinMax = hitAxis(origin.y(), direction.y());
    auto zMinMax = hitAxis(origin.z(), direction.z());
    auto tmin = std::max(std::max(xMinMax.first, yMinMax.first), zMinMax.first);
    auto tmax =
        std::min(std::min(xMinMax.second, yMinMax.second), zMinMax.second);
    if (tmin > tmax) {
      return false;
    }
    record.thit = Intersection::getMinHitParam(transf_ray, {tmin, tmax});
    record.hit_point = record.getHitPoint(transf_ray);
    record.normal = normal(record.hit_point);
    return true;
  }

  Normal3D normal(const Point3D &p) const override {
    auto object_point = m_transformation.worldToObjectSpace(p);
    auto max_coord =
        std::max(std::max(fabs(object_point.x()), fabs(object_point.y())),
                 fabs(object_point.z()));
    Normal3D object_normal;
    if (max_coord == fabs(object_point.x())) {
      object_normal = Normal3D(object_point.x(), 0.f, 0.f);
    } else if (max_coord == fabs(object_point.y())) {
      object_normal = Normal3D(0.f, object_point.y(), 0.f);
    } else {
      object_normal = Normal3D(0.f, 0.f, object_point.z());
    }
    return getUnitVectorOf(m_transformation.objectToWorldSpace(object_normal));
  }

  static PrimitivePtr create(const Transformation &tr = Transformation()) {
    return std::make_shared<Cube>(tr);
  }

 private:
  std::pair<float, float> hitAxis(float origin, float direction) {
    auto tmin_numerator = (-1.f - origin);
    auto tmax_numerator = (1.f - origin);
    float tmin{}, tmax{};
    if (fabs(direction) >= EPS) {
      tmin = tmin_numerator / direction;
      tmax = tmax_numerator / direction;
    } else {
      tmin = tmin_numerator * std::numeric_limits<float>::infinity();
      tmax = tmax_numerator * std::numeric_limits<float>::infinity();
    }
    if (tmin > tmax) {
      std::swap(tmin, tmax);
    }
    return std::make_pair(tmin, tmax);
  }
};
