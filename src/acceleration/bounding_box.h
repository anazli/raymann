#pragma once

#include "math_utils/math_utils.h"

using limit = std::numeric_limits<float>;

class BoundingBox {
 public:
  BoundingBox() = default;
  BoundingBox(const Point3D &pmin, const Point3D &pmax);
  Point3D &minPoint();
  Point3D &maxPoint();
  const Point3D &minPoint() const;
  const Point3D &maxPoint() const;
  void addPoint(const Point3D &point);
  void addBox(const BoundingBox &box);
  bool containsPoint(const Point3D &point) const;
  bool containsBoundingBox(const BoundingBox &box) const;
  bool intersectsRay(const Ray &r) const;

 private:
  std::pair<float, float> hitAxis(float origin, float direction, float min,
                                  float max) const;
  Point3D m_minPoint =
      Point3D(limit::infinity(), limit::infinity(), limit::infinity());
  Point3D m_maxPoint =
      Point3D(-limit::infinity(), -limit::infinity(), -limit::infinity());
};
