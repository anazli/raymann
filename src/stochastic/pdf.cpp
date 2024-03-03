#include "stochastic/pdf.h"

#include "stochastic/random.h"

void StochasticPdf::setFromW(const Vec3f &w) {}

float SpherePdf::value(const Vec3f &direction) const {
  return 1.f / (4.f * PI);
}

Vec3f SpherePdf::generate() const { return Random::randomVectorOnUnitSphere(); }

CosPdf::CosPdf(const Vec3f &w) { onb.buildFromW(w); }

float CosPdf::value(const Vec3f &direction) const {
  auto cosTheta = dot(Vec3f(direction).normalize(), onb.w());
  return fmax(0, cosTheta / PI);
}

Vec3f CosPdf::generate() const {
  return onb.local(Random::randomCosineDirection());
}

void CosPdf::setFromW(const Vec3f &w) { onb.buildFromW(w); }

PrimitivePdf::PrimitivePdf(const SceneElementRawPtr elem, const Point3f &origin)
    : m_element(elem), m_origin(origin) {}

float PrimitivePdf::value(const Vec3f &direction) const {
  return m_element->pdf(m_origin, direction);
}

Vec3f PrimitivePdf::generate() const { return m_element->random(m_origin); }

CombinedPdf::CombinedPdf(StochasticPdfPtr pdf1, StochasticPdfPtr pdf2)
    : m_firstPdf(pdf1), m_secondPdf(pdf2) {}

float CombinedPdf::value(const Vec3f &direction) const {
  if (m_firstPdf && m_secondPdf) {
    return 0.5f * m_firstPdf->value(direction) +
           0.5f * m_secondPdf->value(direction);
  }
  return 1.f;
}

Vec3f CombinedPdf::generate() const {
  if (m_firstPdf && Random::randomNumber() < 0.5f) {
    return m_firstPdf->generate();
  }
  if (m_secondPdf) {
    return m_secondPdf->generate();
  }
  return Vec3f();
}
