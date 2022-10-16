#include "composite/scene_director.h"

#include "geometry/plane.h"

SceneDirector::SceneDirector(std::shared_ptr<TraceableBuilder> builder,
                             const Properties& prop)
    : m_builder(builder), m_properties(prop) {}

StandardSphere::StandardSphere(std::shared_ptr<TraceableBuilder> builder,
                               const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> StandardSphere::create() {
  m_builder->addTraceable(new Sphere);
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addMaterial();
  return m_builder->getTraceable();
}

SphereWithCheckerPattern::SphereWithCheckerPattern(
    std::shared_ptr<TraceableBuilder> builder, const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> SphereWithCheckerPattern::create() {
  m_builder->addTraceable(new Sphere);
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addCheckerPattern();
  return m_builder->getTraceable();
}

SphereWithRingPattern::SphereWithRingPattern(
    std::shared_ptr<TraceableBuilder> builder, const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> SphereWithRingPattern::create() {
  m_builder->addTraceable(new Sphere);
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addRingPattern();
  return m_builder->getTraceable();
}

SphereWithGradientPattern::SphereWithGradientPattern(
    std::shared_ptr<TraceableBuilder> builder, const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> SphereWithGradientPattern::create() {
  m_builder->addTraceable(new Sphere);
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addGradientPattern();
  return m_builder->getTraceable();
}

StandardPlane::StandardPlane(std::shared_ptr<TraceableBuilder> builder,
                             const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> StandardPlane::create() {
  m_builder->addTraceable(new Plane);
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addMaterial();
  return m_builder->getTraceable();
}

PlaneWithCheckerPattern::PlaneWithCheckerPattern(
    std::shared_ptr<TraceableBuilder> builder, const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> PlaneWithCheckerPattern::create() {
  m_builder->addTraceable(new Plane);
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addCheckerPattern();
  return m_builder->getTraceable();
}

PlaneWithRingPattern::PlaneWithRingPattern(
    std::shared_ptr<TraceableBuilder> builder, const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> PlaneWithRingPattern::create() {
  m_builder->addTraceable(new Plane);
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addRingPattern();
  return m_builder->getTraceable();
}
