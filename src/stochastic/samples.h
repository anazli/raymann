// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/).
// pbrt-v2 is distributed under the terms of the BSD 3-Clause License.

#pragma once

#include <math.h>

#include <algorithm>

Vec2f concentricSampleDisk(const Vec2f& u);

inline Vec3f cosineSampleHemisphere(const Vec2f& v) {
  auto ret = concentricSampleDisk(v);
  auto z =
      std::sqrt(std::max(0.f, 1.f - ret.x() * ret.x() - ret.y() * ret.y()));
  return Vec3f(ret.x(), ret.y(), z);
}