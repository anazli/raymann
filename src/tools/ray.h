#pragma once

#include "tools/tools.h"

class Ray {
 public:
  Ray() = default;
  Ray(const Point3f &origin, const Vec3f &direction)
      : m_origin(origin), m_direction(direction) {}

  void setOrigin(const Point3f &origin) { m_origin = origin; }
  void setDirection(const Vec3f &direction) { m_direction = direction; }
  Point3f origin() const { return m_origin; }
  Vec3f direction() const { return m_direction; }
  Point3f position(const float &parameter) const {
    return origin() + parameter * direction();
  }
  Ray transform(const Mat4f &matrix) const {
    Ray transformed_ray;

    Vec4f vector4d(origin());
    vector4d = matrix * vector4d;
    Point3f new_origin = vector4d;
    transformed_ray.setOrigin(new_origin);

    vector4d = direction();
    vector4d = matrix * vector4d;
    Vec4f new_direction = vector4d;
    transformed_ray.setDirection(new_direction);
    return transformed_ray;
  }

 private:
  Point3f m_origin;
  Vec3f m_direction;
};
