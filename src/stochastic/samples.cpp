// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/).
// pbrt-v2 is distributed under the terms of the BSD 3-Clause License.

#include "samples.h"

Vec2f concentricSampleDisk(const Vec2f& u) {
  auto offset = 2.f * u - Vec2f(1.f, 1.f);
  if (offset == Vec2f()) return Vec2f();
  float theta, r;
  if (std::fabs(offset.x()) > std::fabs(offset.y())) {
    r = offset.x();
    theta = PIOver4 * (offset.y() / offset.x());
  } else {
    r = offset.y();
    theta = PIOver2 - PIOver4 * (offset.x() / offset.y());
  }
  return r * Vec2f(std::cos(theta), std::sin(theta));
}