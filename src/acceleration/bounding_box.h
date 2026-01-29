#pragma once

#include <point3.h>
#include <ray.h>

using limit = std::numeric_limits<float>;

class BoundingBox {
 public:
  BoundingBox() = default;
  BoundingBox(const Point3f &pmin, const Point3f &pmax);
  Point3f &minPoint();
  Point3f &maxPoint();
  const Point3f &minPoint() const;
  const Point3f &maxPoint() const;
  void addPoint(const Point3f &point);
  void addBox(const BoundingBox &box);
  bool containsPoint(const Point3f &point) const;
  bool containsBoundingBox(const BoundingBox &box) const;
  bool intersectsRay(const Ray &r) const;

 private:
  std::pair<float, float> hitAxis(float origin, float direction, float min,
                                  float max) const;
  Point3f m_minPoint =
      Point3f(limit::infinity(), limit::infinity(), limit::infinity());
  Point3f m_maxPoint =
      Point3f(-limit::infinity(), -limit::infinity(), -limit::infinity());
};
