#include "builder/shape_builder.h"

Shape::Shape() : m_currentTraceable(nullptr) {}

void Shape::addTraceable(Traceable* tr) { m_currentTraceable = tr; }

void Shape::transformTraceable(const Mat4f& m) {
  m_currentTraceable = new Transformer(m_currentTraceable, m);
}

void Shape::addColor(const Vec3f& c, float reflection) {
  m_currentTraceable = new Material(m_currentTraceable, c, reflection);
}

void Shape::addColor(const Vec3f& c, float reflection, float am, float diff,
                     float spec, float shi) {
  m_currentTraceable =
      new Material(m_currentTraceable, c, reflection, am, diff, spec, shi);
}

void Shape::addStripePattern(const Vec3f& colorA, const Vec3f& colorB,
                             const Mat4f& ptrans, const Mat4f& otrans) {
  m_currentTraceable =
      new StripePattern(m_currentTraceable, colorA, colorB, ptrans, otrans);
}

void Shape::addGradientPattern(const Vec3f& colorA, const Vec3f& colorB,
                               const Mat4f& ptrans, const Mat4f& otrans) {
  m_currentTraceable =
      new GradientPattern(m_currentTraceable, colorA, colorB, ptrans, otrans);
}

void Shape::addRingPattern(const Vec3f& colorA, const Vec3f& colorB,
                           const Mat4f& ptrans, const Mat4f& otrans) {
  m_currentTraceable =
      new RingPattern(m_currentTraceable, colorA, colorB, ptrans, otrans);
}

void Shape::addCheckerPattern(const Vec3f& colorA, const Vec3f& colorB,
                              const Mat4f& ptrans, const Mat4f& otrans) {
  m_currentTraceable =
      new CheckerPattern(m_currentTraceable, colorA, colorB, ptrans, otrans);
}

std::shared_ptr<Traceable> Shape::getTraceable() const {
  std::shared_ptr<Traceable> ret(m_currentTraceable);
  return ret;
}
