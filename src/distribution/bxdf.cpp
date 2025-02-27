// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#include "distribution/bxdf.h"

#include "distribution/utilities.h"
#include "stochastic/samples.h"

Spectrum Bxdf::sampleF(const Vec3D& wo, Vec3D& wi, const Vec2D& p,
                       float& pdf_val) const {
  wi = cosineSampleHemisphere(p);
  if (wo.z() < 0.f) wi.setZ(wi.z() * (-1));
  pdf_val = pdf(wo, wi);
  return f(wo, wi);
}

Spectrum Bxdf::rhd(const Vec3D& wo, int num_samples, Vec2D& samples) const {
  Spectrum r(0.f);
  /*for (int i = 0; i < num_samples; ++i) {
    // Estimate one term of $\rho_\roman{hd}$
    Vec3D wi;
    auto pdf = 0.f;
    Spectrum f = Sample_f(w, &wi, u[i], &pdf);
    if (pdf > 0) r += f * absoluteCosTheta(wi) / pdf;
  }*/
  return r / num_samples;
}

Spectrum Bxdf::rhh(int num_samples, Vec2D& samples1, Vec2D& samples2) const {
  Spectrum r(0.f);
  /*for (int i = 0; i < num_samples; ++i) {
    // Estimate one term of $\rho_\roman{hh}$
    Vec3D wo, wi;
    wo = UniformSampleHemisphere(u1[i]);
    float pdfo = UniformHemispherePdf(), pdfi = 0;
    Spectrum f = Sample_f(wo, &wi, u2[i], &pdfi);
    if (pdfi > 0)
      r += f * absoluteCosTheta(wi) * absoluteCosTheta(wo) / (pdfo * pdfi);
  }*/
  return r / (PI * num_samples);
}

float Bxdf::pdf(const Vec3D& wi, const Vec3D& wo) const {
  return isOnSameHemisphere(wo, wi) ? absoluteCosTheta(wi) * InvPI : 0;
}

Bxdf::Bxdf(const BxdfType& type) : m_bsdf_type(type) {}
