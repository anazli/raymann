#include "builder/shape_builder.h"

ShapeBuilder::~ShapeBuilder()
{

}

void ShapeBuilder::addTraceable(Traceable* tr) { m_currentTraceable = tr; }

void ShapeBuilder::addTransformer() {
  m_currentTraceable =
      new Transformer(m_currentTraceable, m_prop.objTrans());
}

void ShapeBuilder::addMaterial() {
  m_currentTraceable = new Material(
      m_currentTraceable, m_prop.color(), m_prop.reflection(), m_prop.ambient(),
      m_prop.diffuse(), m_prop.specular(), m_prop.shininess());
}

void ShapeBuilder::addStripePattern() {
  m_currentTraceable = new StripePattern(
      m_currentTraceable, m_prop.patColA(), m_prop.patColB(),
      m_prop.objTrans(), m_prop.patTrans());
}

void ShapeBuilder::addGradientPattern() {
  m_currentTraceable = new GradientPattern(
      m_currentTraceable, m_prop.patColA(), m_prop.patColB(),
      m_prop.objTrans(), m_prop.patTrans());
}

void ShapeBuilder::addRingPattern() {
  m_currentTraceable = new RingPattern(
      m_currentTraceable, m_prop.patColA(), m_prop.patColB(),
      m_prop.objTrans(), m_prop.patTrans());
}

void ShapeBuilder::addCheckerPattern() {
  m_currentTraceable = new CheckerPattern(
      m_currentTraceable, m_prop.patColA(), m_prop.patColB(),
      m_prop.objTrans(), m_prop.patTrans());
}

std::shared_ptr<Traceable> ShapeBuilder::getTraceable() const {
  std::shared_ptr<Traceable> ret(m_currentTraceable);
  return ret;
}
