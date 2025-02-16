// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/).
// pbrt-v2 is distributed under the terms of the BSD 3-Clause License.

#pragma once

#include <algorithm>

#include "tools/tools.h"

Vec2D concentricSampleDisk(const Vec2D& u);

inline Vec3D cosineSampleHemisphere(const Vec2D& v) {
  auto ret = concentricSampleDisk(v);
  auto z =
      std::sqrt(std::max(0.f, 1.f - ret.x() * ret.x() - ret.y() * ret.y()));
  return Vec3D(ret.x(), ret.y(), z);
}