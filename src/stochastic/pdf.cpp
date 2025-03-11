#include "stochastic/pdf.h"

#include "stochastic/random.h"

void StochasticPdf::setFromW(const Vec3D &w) {}

float SpherePdf::value(const Vec3D &direction) const {
  return 1.f / (4.f * PI);
}

Vec3D SpherePdf::generate() const { return Random::randomVectorOnUnitSphere(); }

CosPdf::CosPdf(const Vec3D &w) { onb.buildFromW(w); }

float CosPdf::value(const Vec3D &direction) const {
  auto cosTheta = dot(getUnitVectorOf(direction), onb.w());
  return fmax(0, cosTheta / PI);
}

Vec3D CosPdf::generate() const {
  return onb.local(Random::randomCosineDirection());
}

void CosPdf::setFromW(const Vec3D &w) { onb.buildFromW(w); }

PrimitivePdf::PrimitivePdf(const SceneElementRawPtr elem, const Point3D &origin)
    : m_element(elem), m_origin(origin) {}

float PrimitivePdf::value(const Vec3D &direction) const { return 1.f; }

Vec3D PrimitivePdf::generate() const { return Vec3D(1.f, 1.f, 1.f); }

CombinedPdf::CombinedPdf(StochasticPdfPtr pdf1, StochasticPdfPtr pdf2,
                         float ratio)
    : m_firstPdf(pdf1), m_secondPdf(pdf2), m_ratio(ratio) {}

float CombinedPdf::value(const Vec3D &direction) const {
  if (m_firstPdf && m_secondPdf) {
    return 0.5f * m_firstPdf->value(direction) +
           0.5f * m_secondPdf->value(direction);
  }
  return 1.f;
}

Vec3D CombinedPdf::generate() const {
  if (m_firstPdf && Random::randomNumber() < m_ratio) {
    return m_firstPdf->generate();
  } else if (m_secondPdf) {
    return m_secondPdf->generate();
  }
  return Vec3D();
}
