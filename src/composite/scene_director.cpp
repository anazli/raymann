#include "composite/scene_director.h"

#include "geometry/plane.h"

std::shared_ptr<Traceable> StandardSphere::create(
    std::shared_ptr<TraceableBuilder> builder, const Properties &prop) {
  builder->addTraceable(
      new Sphere(prop.getPropertyAsPoint3f(PropertyNames::SPHERE_CENTER),
                 prop.getPropertyAsFloat(PropertyNames::SPHERE_RADIUS)));
  builder->setProperties(prop);
  builder->addTransformer();
  builder->addMaterial();
  return builder->getTraceable();
}

std::shared_ptr<Traceable> CheckerSphere::create(
    std::shared_ptr<TraceableBuilder> builder, const Properties &prop) {
  builder->addTraceable(
      new Sphere(prop.getPropertyAsPoint3f(PropertyNames::SPHERE_CENTER),
                 prop.getPropertyAsFloat(PropertyNames::SPHERE_RADIUS)));
  builder->setProperties(prop);
  builder->addTransformer();
  builder->addCheckerPattern();
  return builder->getTraceable();
}

std::shared_ptr<Traceable> RingSphere::create(
    std::shared_ptr<TraceableBuilder> builder, const Properties &prop) {
  builder->addTraceable(
      new Sphere(prop.getPropertyAsPoint3f(PropertyNames::SPHERE_CENTER),
                 prop.getPropertyAsFloat(PropertyNames::SPHERE_RADIUS)));
  builder->setProperties(prop);
  builder->addTransformer();
  builder->addRingPattern();
  return builder->getTraceable();
}

std::shared_ptr<Traceable> GradientSphere::create(
    std::shared_ptr<TraceableBuilder> builder, const Properties &prop) {
  builder->addTraceable(
      new Sphere(prop.getPropertyAsPoint3f(PropertyNames::SPHERE_CENTER),
                 prop.getPropertyAsFloat(PropertyNames::SPHERE_RADIUS)));
  builder->setProperties(prop);
  builder->addTransformer();
  builder->addGradientPattern();
  return builder->getTraceable();
}

std::shared_ptr<Traceable> StandardPlane::create(
    std::shared_ptr<TraceableBuilder> builder, const Properties &prop) {
  builder->addTraceable(new Plane);
  builder->setProperties(prop);
  builder->addTransformer();
  builder->addMaterial();
  return builder->getTraceable();
}

std::shared_ptr<Traceable> CheckerPlane::create(
    std::shared_ptr<TraceableBuilder> builder, const Properties &prop) {
  builder->addTraceable(new Plane);
  builder->setProperties(prop);
  builder->addTransformer();
  builder->addCheckerPattern();
  return builder->getTraceable();
}

std::shared_ptr<Traceable> RingPlane::create(
    std::shared_ptr<TraceableBuilder> builder, const Properties &prop) {
  builder->addTraceable(new Plane);
  builder->setProperties(prop);
  builder->addTransformer();
  builder->addRingPattern();
  return builder->getTraceable();
}
