// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#pragma once

#include <limits>

#include "tools/ray.h"

class SceneElement;

struct IntersectionParameters {
  Point3D hit_point;
  Vec2D uv;
  Vec3D wo;
  Vec3D dpdu, dpdv;
  Normal3D dndu, dndv;
  Normal3D n;
};

class Intersection {
 public:
  Intersection() = default;
  Intersection(const IntersectionParameters &parameters);
  struct {
    Normal3D n;
    Vec3D dpdu, dpdv;
    Normal3D dndu, dndv;
  } ShadingGeometry;
  // get the minimum positive hit parameter from an intersection
  static float getMinimumHitParameter(float t1, float t2);
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
  float minHitParam = std::numeric_limits<float>::max();
  bool hitFound = false;
  float max_ray_range = std::numeric_limits<float>::infinity();
};