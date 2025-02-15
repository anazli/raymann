#include "composite/intersection.h"

#include "intersection.h"

Intersection::Intersection(const IntersectionParameters& parameters) {
  ShadingGeometry.n = surface_normal;
  ShadingGeometry.dpdu = parameters.dpdu;
  ShadingGeometry.dpdv = parameters.dpdv;
  ShadingGeometry.dndu = parameters.dndu;
  ShadingGeometry.dndv = parameters.dndv;
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