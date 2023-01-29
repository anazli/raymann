#include "builder/shape_builder.h"

ShapeBuilder::~ShapeBuilder() {}

void ShapeBuilder::addTraceable(Traceable* tr) { m_currentTraceable = tr; }

void ShapeBuilder::addTransformer() {
  m_currentTraceable = new Transformer(
      m_currentTraceable,
      m_prop.getPropertyAsMat4f(PropertyNames::OBJECT_TRANSFROM_MATRIX));
}

void ShapeBuilder::addMaterial() {
  m_currentTraceable = new Material(
      m_currentTraceable, m_prop.getPropertyAsVec3f(PropertyNames::COLOR),
      m_prop.getPropertyAsFloat(PropertyNames::REFLECTION),
      m_prop.getPropertyAsFloat(PropertyNames::AMBIENT),
      m_prop.getPropertyAsFloat(PropertyNames::DIFFUSE),
      m_prop.getPropertyAsFloat(PropertyNames::SPECULAR),
      m_prop.getPropertyAsFloat(PropertyNames::SHININESS));
}

void ShapeBuilder::addStripePattern() {
  m_currentTraceable = new StripePattern(
      m_currentTraceable,
      m_prop.getPropertyAsVec3f(PropertyNames::PATTERN_COLORA),
      m_prop.getPropertyAsVec3f(PropertyNames::PATTERN_COLORB),
      m_prop.getPropertyAsMat4f(PropertyNames::OBJECT_TRANSFROM_MATRIX),
      m_prop.getPropertyAsMat4f(PropertyNames::PATTERN_TRANSFORM_MATRIX));
}

void ShapeBuilder::addGradientPattern() {
  m_currentTraceable = new GradientPattern(
      m_currentTraceable,
      m_prop.getPropertyAsVec3f(PropertyNames::PATTERN_COLORA),
      m_prop.getPropertyAsVec3f(PropertyNames::PATTERN_COLORB),
      m_prop.getPropertyAsMat4f(PropertyNames::OBJECT_TRANSFROM_MATRIX),
      m_prop.getPropertyAsMat4f(PropertyNames::PATTERN_TRANSFORM_MATRIX));
}

void ShapeBuilder::addRingPattern() {
  m_currentTraceable = new RingPattern(
      m_currentTraceable,
      m_prop.getPropertyAsVec3f(PropertyNames::PATTERN_COLORA),
      m_prop.getPropertyAsVec3f(PropertyNames::PATTERN_COLORB),
      m_prop.getPropertyAsMat4f(PropertyNames::OBJECT_TRANSFROM_MATRIX),
      m_prop.getPropertyAsMat4f(PropertyNames::PATTERN_TRANSFORM_MATRIX));
}

void ShapeBuilder::addCheckerPattern() {
  m_currentTraceable = new CheckerPattern(
      m_currentTraceable,
      m_prop.getPropertyAsVec3f(PropertyNames::PATTERN_COLORA),
      m_prop.getPropertyAsVec3f(PropertyNames::PATTERN_COLORB),
      m_prop.getPropertyAsMat4f(PropertyNames::OBJECT_TRANSFROM_MATRIX),
      m_prop.getPropertyAsMat4f(PropertyNames::PATTERN_TRANSFORM_MATRIX),
      m_prop.getPropertyAsVec3f(PropertyNames::COLOR),
      m_prop.getPropertyAsFloat(PropertyNames::REFLECTION),
      m_prop.getPropertyAsFloat(PropertyNames::AMBIENT),
      m_prop.getPropertyAsFloat(PropertyNames::DIFFUSE),
      m_prop.getPropertyAsFloat(PropertyNames::SPECULAR),
      m_prop.getPropertyAsFloat(PropertyNames::SHININESS));
}

std::shared_ptr<Traceable> ShapeBuilder::getTraceable() const {
  std::shared_ptr<Traceable> ret(m_currentTraceable);
  return ret;
}
