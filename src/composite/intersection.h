#pragma once

#include <limits>

#include "tools/ray.h"

class SceneElement;

class Intersection {
 public:
  float t1 = -limit::max();
  float t2 = -limit::max();
  float t_min() const {
    if ((t1 < limit::max() && t1 > 0.001f) ||
        (t2 < limit::max() && t2 > 0.001f)) {
      if (t1 <= 0.0f && t2 > 0.0f)
        return t2;
      else if (t2 <= 0.0f && t1 > 0.0f)
        return t1;
      else if (t1 > 0.0f && t2 > 0.0f)
        return std::min(t1, t2);
    }
    return -1.0f;  // TODO: to be fixed for negative intersections
  }
  Point3D point(const Ray &r) const { return r.position(t_min()); }
  Point3D saved_point;
  Vec3D eye(const Ray &r) const { return -r.direction(); }
  bool inside = false;
  Point3D over_point_from_refl_surf;
  Point3D under_point_from_refrac_surf;
  Vec3D surface_normal;
  SceneElement *primitive = nullptr;
  float minHitParam = limit::max();
  bool hitFound = false;
  float max_ray_range = std::numeric_limits<float>::infinity();
};