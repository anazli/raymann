#pragma once

#include "math_utils/math_utils.h"

class Ray {
 public:
  Ray() = default;
  Ray(const Point3D &origin, const Vec3D &direction)
      : m_origin(origin), m_direction(direction) {}

  void setOrigin(const Point3D &origin) { m_origin = origin; }
  void setDirection(const Vec3D &direction) { m_direction = direction; }
  Point3D origin() const { return m_origin; }
  Vec3D direction() const { return m_direction; }
  Point3D position(const float &parameter) const {
    return origin() + parameter * direction();
  }

  void setMinRange(float t) { m_min_parameter = t; }
  float getMinRange() const { return m_min_parameter; }
  void setMaxRange(float t) { m_max_parameter = t; }
  float getMaxRange() const { return m_max_parameter; }

 private:
  Point3D m_origin;
  Vec3D m_direction;
  mutable float m_min_parameter = 0.01f;
  mutable float m_max_parameter = std::numeric_limits<float>::infinity();
};
