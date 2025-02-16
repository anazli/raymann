// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/).
// pbrt-v2 is distributed under the terms of the BSD 3-Clause License.

#include "samples.h"

Vec2D concentricSampleDisk(const Vec2D& u) {
  auto offset = 2.f * u - Vec2D(1.f, 1.f);
  if (offset == Vec2D()) return Vec2D();
  float theta, r;
  if (std::fabs(offset.x()) > std::fabs(offset.y())) {
    r = offset.x();
    theta = PIOver4 * (offset.y() / offset.x());
  } else {
    r = offset.y();
    theta = PIOver2 - PIOver4 * (offset.x() / offset.y());
  }
  return r * Vec2D(std::cos(theta), std::sin(theta));
}