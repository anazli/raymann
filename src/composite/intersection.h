#pragma once

#include <limits>

#include "tools/ray.h"

class SceneElement;

class Intersection {
 public:
  // get the minimum positive hit parameter from an intersection
  static float getMinimumHitParameter(float t1, float t2) {
    auto t1_valid = (t1 > 0.001f);
    auto t2_valid = (t2 > 0.001f);
    if (!t1_valid && !t2_valid) {
      return std::numeric_limits<float>::infinity();
    }
    if (t1_valid && !t2_valid) {
      return t1;
    } else if (!t1_valid && t2_valid) {
      return t2;
    }
    return std::min(t1, t2);
  }

  float min_hit = std::numeric_limits<float>::infinity();
  Point3D getHitPoint(const Ray &r) const { return r.position(min_hit); }
  // point of intersection test in world space
  Point3D hit_point;
  // negative ray direction
  Vec3D omega;
  bool inside = false;
  Point3D over_point_from_refl_surf;
  Point3D under_point_from_refrac_surf;
  // normal of closest hit in world space
  Normal3D surface_normal;
  // primitive of closest hit
  SceneElement *primitive = nullptr;
  float minHitParam = limit::max();
  bool hitFound = false;
  float max_ray_range = std::numeric_limits<float>::infinity();
};