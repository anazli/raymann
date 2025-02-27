// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#include "composite/intersection.h"

#include "intersection.h"

Intersection::Intersection(const IntersectionParameters& parameters) {
  ShadingGeometry.n = surface_normal;
  ShadingGeometry.dpdu = parameters.dpdu;
  ShadingGeometry.dpdv = parameters.dpdv;
  ShadingGeometry.dndu = parameters.dndu;
  ShadingGeometry.dndv = parameters.dndv;
}

void Intersection::computeDifferentials(const Ray& r) {
  dudx = dvdx = 0.f;
  dudy = dvdy = 0.f;
  dpdx = dpdy = Vec3D(0.f, 0.f, 0.f);
}

void Intersection::evaluateScattering(const Ray& r) {
  computeDifferentials(r);
  // primitive->evaluateScattering();
}

float Intersection::getMinimumHitParameter(float t1, float t2) {
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