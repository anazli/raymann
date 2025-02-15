// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#include "bsdf.h"

Bsdf::Bsdf(const Intersection& record, float relative_refract_index) {}

void Bsdf::add(BxdfPtr bxdf) { m_bxdf_container.emplace_back(bxdf); }
