#include "builder/shape_builder.h"

ShapeBuilder::~ShapeBuilder() {}

void ShapeBuilder::addTraceable(Traceable* tr) { m_currentTraceable = tr; }

void ShapeBuilder::addTransformer() {
  m_currentTraceable = new Transformer(
      m_currentTraceable,
      m_prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX));
}

void ShapeBuilder::addMaterial() {
  m_currentTraceable = new Material(
      m_currentTraceable, m_prop.getPropertyAsVec3f(Props::COLOR),
      m_prop.getPropertyAsFloat(Props::REFLECTION),
      m_prop.getPropertyAsFloat(Props::AMBIENT),
      m_prop.getPropertyAsFloat(Props::DIFFUSE),
      m_prop.getPropertyAsFloat(Props::SPECULAR),
      m_prop.getPropertyAsFloat(Props::SHININESS));
}

void ShapeBuilder::addStripePattern() {
  m_currentTraceable = new StripePattern(
      m_currentTraceable,
      m_prop.getPropertyAsVec3f(Props::PATTERN_COLORA),
      m_prop.getPropertyAsVec3f(Props::PATTERN_COLORB),
      m_prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX),
      m_prop.getPropertyAsMat4f(Props::PATTERN_TRANSFORM_MATRIX));
}

void ShapeBuilder::addGradientPattern() {
  m_currentTraceable = new GradientPattern(
      m_currentTraceable,
      m_prop.getPropertyAsVec3f(Props::PATTERN_COLORA),
      m_prop.getPropertyAsVec3f(Props::PATTERN_COLORB),
      m_prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX),
      m_prop.getPropertyAsMat4f(Props::PATTERN_TRANSFORM_MATRIX));
}

void ShapeBuilder::addRingPattern() {
  m_currentTraceable = new RingPattern(
      m_currentTraceable,
      m_prop.getPropertyAsVec3f(Props::PATTERN_COLORA),
      m_prop.getPropertyAsVec3f(Props::PATTERN_COLORB),
      m_prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX),
      m_prop.getPropertyAsMat4f(Props::PATTERN_TRANSFORM_MATRIX));
}

void ShapeBuilder::addCheckerPattern() {
  m_currentTraceable = new CheckerPattern(
      m_currentTraceable,
      m_prop.getPropertyAsVec3f(Props::PATTERN_COLORA),
      m_prop.getPropertyAsVec3f(Props::PATTERN_COLORB),
      m_prop.getPropertyAsMat4f(Props::OBJECT_TRANSFROM_MATRIX),
      m_prop.getPropertyAsMat4f(Props::PATTERN_TRANSFORM_MATRIX),
      m_prop.getPropertyAsVec3f(Props::COLOR),
      m_prop.getPropertyAsFloat(Props::REFLECTION),
      m_prop.getPropertyAsFloat(Props::AMBIENT),
      m_prop.getPropertyAsFloat(Props::DIFFUSE),
      m_prop.getPropertyAsFloat(Props::SPECULAR),
      m_prop.getPropertyAsFloat(Props::SHININESS));
}

std::shared_ptr<Traceable> ShapeBuilder::getTraceable() const {
  std::shared_ptr<Traceable> ret(m_currentTraceable);
  return ret;
}
