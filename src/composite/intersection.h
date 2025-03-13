// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#pragma once

#include <limits>

#include "tools/ray.h"

class SceneElement;

struct Intersection {
  // get the minimum positive hit parameter from an intersection
  static float getMinHitParam(const Ray& r, std::vector<float>&& tparams) {
    auto thit = r.getMaxRange();
    for (const auto t : tparams) {
      if (t >= r.getMinRange() && t < thit) thit = t;
    }
    return thit;
  }
  float thit = std::numeric_limits<float>::infinity();
  Point3D getHitPoint(const Ray& r) const { return r.position(thit); }
  // point of intersection test in world space
  Point3D hit_point;
  // negative ray direction
  Vec3D omega;
  // geometric normal of closest hit point in world space
  Normal3D normal;
  // primitive of closest hit
  SceneElement* closest_scene_element = nullptr;
};