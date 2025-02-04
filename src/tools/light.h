#pragma once

#include "point3.h"
#include "vec3.h"

class PointLight {
 public:
  PointLight() = default;
  PointLight(const Point3f &pos, const Vec3f &inten)
      : m_position(pos), m_intensity(inten) {}

  void setPosition(const Point3f &pos) { m_position = pos; }
  void setIntensity(const Vec3f &inten) { m_intensity = inten; }
  Point3f position() const { return m_position; }
  Vec3f intensity() const { return m_intensity; }

 private:
  Point3f m_position;
  Vec3f m_intensity;
};
