#pragma once

#include "point3.h"
#include "vec3.h"

class PointLight {
 public:
  PointLight() = default;
  PointLight(const Point3D &pos, const Vec3D &inten)
      : m_position(pos), m_intensity(inten) {}

  void setPosition(const Point3D &pos) { m_position = pos; }
  void setIntensity(const Vec3D &inten) { m_intensity = inten; }
  Point3D position() const { return m_position; }
  Vec3D intensity() const { return m_intensity; }

 private:
  Point3D m_position;
  Vec3D m_intensity;
};
