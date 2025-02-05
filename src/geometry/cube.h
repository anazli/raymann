#pragma once

#include <limits>

#include "composite/scene_element.h"
#include "stochastic/random.h"

class Cube : public SceneElement {
 public:
  Cube() {
    m_elementType = SceneElementType::CUBE;
    m_bBox.minPoint() = Point3D(-1.f, -1.f, -1.f);
    m_bBox.maxPoint() = Point3D(1.f, 1.f, 1.f);
  }
  ~Cube() override = default;

  bool intersect(const Ray &r, IntersectionRecord &record) override {
    std::pair<float, float> xMinMax =
        hitAxis(r.origin().x(), r.direction().x());
    std::pair<float, float> yMinMax =
        hitAxis(r.origin().y(), r.direction().y());
    std::pair<float, float> zMinMax =
        hitAxis(r.origin().z(), r.direction().z());
    auto tmin = std::max(std::max(xMinMax.first, yMinMax.first), zMinMax.first);
    auto tmax =
        std::min(std::min(xMinMax.second, yMinMax.second), zMinMax.second);
    if (tmin > tmax) {
      return false;
    }
    record.t1 = tmin;
    record.t2 = tmax;
    record.count = 2;
    record.saved_point = record.point(r);
    return true;
  }

  Vec3D normal(const Point3D &p) const override {
    auto max_coord = std::max(std::max(fabs(p.x()), fabs(p.y())), fabs(p.z()));
    if (max_coord == fabs(p.x()))
      return Vec3D(p.x(), 0.f, 0.f);
    else if (max_coord == fabs(p.y()))
      return Vec3D(0.f, p.y(), 0.f);
    return Vec3D(0.f, 0.f, p.z());
  }

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
