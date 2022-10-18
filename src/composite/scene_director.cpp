#include "composite/scene_director.h"

#include "geometry/plane.h"

SceneDirector::SceneDirector(std::shared_ptr<TraceableBuilder> builder,
                             const Properties& prop)
    : m_builder(builder), m_properties(prop) {}

StandardSphere::StandardSphere(std::shared_ptr<TraceableBuilder> builder,
                               const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> StandardSphere::create() {
  m_builder->addTraceable(
      new Sphere(m_properties.sphereCenter(), m_properties.sphereRadius()));
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addMaterial();
  return m_builder->getTraceable();
}

CheckerSphere::CheckerSphere(std::shared_ptr<TraceableBuilder> builder,
                             const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> CheckerSphere::create() {
  m_builder->addTraceable(
      new Sphere(m_properties.sphereCenter(), m_properties.sphereRadius()));
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addCheckerPattern();
  return m_builder->getTraceable();
}

RingSphere::RingSphere(std::shared_ptr<TraceableBuilder> builder,
                       const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> RingSphere::create() {
  m_builder->addTraceable(
      new Sphere(m_properties.sphereCenter(), m_properties.sphereRadius()));
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addRingPattern();
  return m_builder->getTraceable();
}

GradientSphere::GradientSphere(std::shared_ptr<TraceableBuilder> builder,
                               const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> GradientSphere::create() {
  m_builder->addTraceable(
      new Sphere(m_properties.sphereCenter(), m_properties.sphereRadius()));
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

CheckerPlane::CheckerPlane(std::shared_ptr<TraceableBuilder> builder,
                           const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> CheckerPlane::create() {
  m_builder->addTraceable(new Plane);
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addCheckerPattern();
  return m_builder->getTraceable();
}

RingPlane::RingPlane(std::shared_ptr<TraceableBuilder> builder,
                     const Properties& prop)
    : SceneDirector(builder, prop) {}

std::shared_ptr<Traceable> RingPlane::create() {
  m_builder->addTraceable(new Plane);
  m_builder->setProperties(m_properties);
  m_builder->addTransformer();
  m_builder->addRingPattern();
  return m_builder->getTraceable();
}
