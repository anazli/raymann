#pragma once

#include "tools/point3.h"
#include "tools/ray.h"
#include "traceable.h"

class Sphere : public Traceable {
 public:
  Sphere(const Point3f &c = Point3f(0.0f, 0.0f, 0.0f), const float &r = 0.0f)
      : m_center(c), m_radius(r) {}

  virtual ~Sphere() {}
  bool intersect(const Ray &r) override { return false; }
  void setCenter(const Point3f &c) { m_center = c; }
  void setRadius(const float &r) { m_radius = r; }
  Point3f center() const { return m_center; }
  float radius() const { return m_radius; }
  std::vector<float> intersections() const { return m_intersections; }

 private:
  Point3f m_center;
  float m_radius;
  std::vector<float> m_intersections;
};
