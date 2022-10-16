#include "builder/shape_builder.h"

void StandardBuilder::addTraceable(Traceable* tr) { m_currentTraceable = tr; }

void StandardBuilder::addTransformer() {
  m_currentTraceable =
      new Transformer(m_currentTraceable, m_prop.objectTransformation());
}

void StandardBuilder::addMaterial() {
  m_currentTraceable = new Material(
      m_currentTraceable, m_prop.color(), m_prop.reflection(), m_prop.ambient(),
      m_prop.diffuse(), m_prop.specular(), m_prop.shininess());
}

void StandardBuilder::addStripePattern() {
  m_currentTraceable = new StripePattern(
      m_currentTraceable, m_prop.patternColorA(), m_prop.patternColorB(),
      m_prop.objectTransformation(), m_prop.patternTransformation());
}

void StandardBuilder::addGradientPattern() {
  m_currentTraceable = new GradientPattern(
      m_currentTraceable, m_prop.patternColorA(), m_prop.patternColorB(),
      m_prop.objectTransformation(), m_prop.patternTransformation());
}

void StandardBuilder::addRingPattern() {
  m_currentTraceable = new RingPattern(
      m_currentTraceable, m_prop.patternColorA(), m_prop.patternColorB(),
      m_prop.objectTransformation(), m_prop.patternTransformation());
}

void StandardBuilder::addCheckerPattern() {
  m_currentTraceable = new CheckerPattern(
      m_currentTraceable, m_prop.patternColorA(), m_prop.patternColorB(),
      m_prop.objectTransformation(), m_prop.patternTransformation());
}

std::shared_ptr<Traceable> StandardBuilder::getTraceable() const {
  std::shared_ptr<Traceable> ret(m_currentTraceable);
  return ret;
}
