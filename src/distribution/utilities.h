// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#pragma once

#include "math_utils/math_utils.h"

inline bool isOnSameHemisphere(const Vec3f& v1, const Vec3f& v2) {
  return v1.z() * v2.z() > 0.f;
}

inline bool isOnSameHemisphere(const Vec3f& v, const Normal3f& n) {
  return v.z() * n.z() > 0.f;
}

inline float absoluteCosTheta(const Vec3f& v) { return std::abs(v.z()); }