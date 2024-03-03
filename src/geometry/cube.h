#pragma once

#include <limits>

#include "composite/scene_element.h"
#include "stochastic/random.h"

class Cube : public SceneElement {
 public:
  Cube() {
    m_elementType = SceneElementType::CUBE;
    m_bBox.minPoint() = Point3f(-1.f, -1.f, -1.f);
    m_bBox.maxPoint() = Point3f(1.f, 1.f, 1.f);
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
    return true;
  }

  Vec3f normal(const Point3f &p) const override {
    auto max_coord = std::max(std::max(fabs(p.x()), fabs(p.y())), fabs(p.z()));
    if (max_coord == fabs(p.x()))
      return Vec3f(p.x(), 0.f, 0.f);
    else if (max_coord == fabs(p.y()))
      return Vec3f(0.f, p.y(), 0.f);
    return Vec3f(0.f, 0.f, p.z());
  }

  float pdf(const Point3f &origin, const Vec3f &direction) override {
    IntersectionRecord rec;
    auto minp = boundingBox().minPoint();
    auto maxp = boundingBox().maxPoint();
    auto area = (maxp.x() - minp.x()) * (maxp.z() - minp.z());
    if (!intersect(Ray(origin, direction), rec)) return 1.f;

    auto distSquared =
        rec.t_min() * rec.t_min() * direction.length() * direction.length();
    auto cosine = fabs(dot(direction, normal(origin)) / direction.length());

    return distSquared / (cosine * area);
  }
  Vec3f random(const Point3f &origin) override {
    auto minp = boundingBox().minPoint();
    auto maxp = boundingBox().maxPoint();
    auto p = minp + (Random::randomNumber() * (maxp.x() - minp.x())) +
             (Random::randomNumber() * (maxp.z() - minp.z()));
    return p - origin;
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
