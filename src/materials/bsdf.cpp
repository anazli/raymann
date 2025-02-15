#include "bsdf.h"

Bsdf::Bsdf(const Intersection& record, float relative_refract_index) {}

void Bsdf::add(BxdfPtr bxdf) { m_bxdf_container.emplace_back(bxdf); }
