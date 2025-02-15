// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#include "distribution/utilities.h"

#include "utilities.h"

inline bool isOnSameHemisphere(const Vec3D& v1, const Vec3D& v2) {
  return v1.z() * v2.z() > 0.f;
}

inline bool isOnSameHemisphere(const Vec3D& v, const Normal3D& n) {
  return v.z() * n.z() > 0.f;
}

inline float absoluteCosTheta(const Vec3D& v) { return std::abs(v.z()); }