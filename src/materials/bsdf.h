// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/).
// pbrt-v2 is distributed under the terms of the BSD 3-Clause License.

#pragma once
#include "composite/intersection.h"
#include "distribution/bxdf.h"
#include "tools/normal3.h"

class Bsdf {
 public:
  Bsdf(const Intersection& record, float relative_refract_index = 1.f);
  void add(BxdfPtr bxdf);

 private:
  Normal3D m_shading_normal, m_geometric_normal;
  Vec3D ss, ts;
  std::vector<BxdfPtr> m_bxdf_container;
};