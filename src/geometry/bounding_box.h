#pragma once

#include <limits>

#include "composite/scene_element.h"

class BoundingBox : public SceneElement {
 public:
  ~BoundingBox() override = default;
  bool intersect(const Ray &r, IntersectionRecord &record) override {
    std::pair<float, float> xMinMax =
        hitAxis(r.origin().x(), r.direction().x());
    std::pair<float, float> yMinMax =
        hitAxis(r.origin().y(), r.direction().y());
    std::pair<float, float> zMinMax =
        hitAxis(r.origin().z(), r.direction().z());

    float tmin =
        std::max(std::max(xMinMax.first, yMinMax.first), zMinMax.first);
    float tmax =
        std::min(std::min(xMinMax.second, yMinMax.second), zMinMax.second);

    if (tmin > tmax) {
      return false;
    }

    record.t1 = tmin;
    record.t2 = tmax;
    record.count = 2;
    return true;
  }

 private:
  std::pair<float, float> hitAxis(float origin, float direction) {
    float tmin_numerator = (-1.f - origin);
    float tmax_numerator = (1.f - origin);

    float tmin{0.f}, tmax{0.f};
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