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