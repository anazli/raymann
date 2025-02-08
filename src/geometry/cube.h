#pragma once

#include <limits>

#include "composite/scene_element.h"
#include "stochastic/random.h"

class Cube : public SceneElement {
 public:
  Cube() {
    m_bBox.minPoint() = Point3D(-1.f, -1.f, -1.f);
    m_bBox.maxPoint() = Point3D(1.f, 1.f, 1.f);
  }
  ~Cube() override = default;

  bool intersect(const Ray &r, IntersectionRecord &record) override {
    auto transformed_ray = r.transform(m_transformation.getInverseMatrix());
    auto origin = transformed_ray.origin();
    auto direction = transformed_ray.direction();
    auto xMinMax = hitAxis(origin.x(), direction.x());
    auto yMinMax = hitAxis(origin.y(), direction.y());
    auto zMinMax = hitAxis(origin.z(), direction.z());
    auto tmin = std::max(std::max(xMinMax.first, yMinMax.first), zMinMax.first);
    auto tmax =
        std::min(std::min(xMinMax.second, yMinMax.second), zMinMax.second);
    if (tmin > tmax) {
      return false;
    }
    record.t1 = tmin;
    record.t2 = tmax;
    record.count = 2;
    record.saved_point = record.point(transformed_ray);
    return true;
  }

  Vec3D normal(const Point3D &p) const override {
    Vec4D v4 = p;
    auto object_point = m_transformation.getInverseMatrix() * v4;
    auto max_coord =
        std::max(std::max(fabs(object_point.x()), fabs(object_point.y())),
                 fabs(object_point.z()));
    Vec3D object_normal;
    if (max_coord == fabs(object_point.x()))
      object_normal = Vec3D(object_point.x(), 0.f, 0.f);
    else if (max_coord == fabs(object_point.y()))
      object_normal = Vec3D(0.f, object_point.y(), 0.f);
    else
      object_normal = Vec3D(0.f, 0.f, object_point.z());
    auto world_normal =
        m_transformation.getInverseMatrix() * Vec4D(object_normal);
    return getUnitVectorOf(world_normal);
  }

  static SceneElementPtr create() { return std::make_shared<Cube>(); }

 private:
  std::pair<float, float> hitAxis(float origin, float direction) {
    auto tmin_numerator = (-1.f - origin);
    auto tmax_numerator = (1.f - origin);
    auto tmin{0.f}, tmax{0.f};
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
