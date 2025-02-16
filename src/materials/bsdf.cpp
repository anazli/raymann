// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#include "bsdf.h"

#include <algorithm>

Bsdf::Bsdf(const Intersection& record, float relative_refract_index)
    : m_geometric_normal(record.surface_normal),
      m_shading_normal(record.ShadingGeometry.n),
      ss(getUnitVectorOf(record.ShadingGeometry.dpdu)),
      ts(cross(Vec3D(m_shading_normal), ss)) {}

void Bsdf::add(BxdfPtr bxdf) { m_bxdf_container.emplace_back(bxdf); }

Vec3D Bsdf::worldToLocal(const Vec3D& v) {
  return Vec3D(dot(v, ss), dot(v, ts), dot(v, Vec3D(m_shading_normal)));
}

Vec3D Bsdf::localToWorld(const Vec3D& v) {
  return Vec3D(ss.x() * v.x() + ts.x() * v.y() + m_shading_normal.x() * v.z(),
               ss.y() * v.x() + ts.y() * v.y() + m_shading_normal.y() * v.z(),
               ss.z() * v.x() + ts.z() * v.y() + m_shading_normal.z() * v.z());
}

Spectrum Bsdf::f(const Vec3D& world_outgoing, const Vec3D& world_incident) {
  auto local_incident = worldToLocal(world_incident);
  auto local_outgoing = worldToLocal(world_outgoing);
  auto should_reflect = dot(world_incident, m_geometric_normal) *
                            dot(world_outgoing, m_geometric_normal) >
                        0.f;
  Spectrum spectrum(0.f);
  for (auto bxdf : m_bxdf_container) {
    if (should_reflect) {
      spectrum += bxdf->f(local_outgoing, local_incident);
    }
  }
  return spectrum;
}

Spectrum Bsdf::rhd(const Vec3D& wo, int num_samples, Vec2D& samples) {
  return std::accumulate(
      m_bxdf_container.begin(), m_bxdf_container.end(), Spectrum(0.f),
      [&](Spectrum accumulated_spectrum, BxdfPtr element) {
        return accumulated_spectrum + element->rhd(wo, num_samples, samples);
      });
}

Spectrum Bsdf::rhh(int num_samples, Vec2D& samples1, Vec2D& samples2) {
  return std::accumulate(m_bxdf_container.begin(), m_bxdf_container.end(),
                         Spectrum(0.f),
                         [&](Spectrum accumulated_spectrum, BxdfPtr element) {
                           return accumulated_spectrum +
                                  element->rhh(num_samples, samples1, samples2);
                         });
}
