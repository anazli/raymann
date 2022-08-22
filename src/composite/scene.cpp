#include "composite/scene.h"

std::shared_ptr<Traceable> Scene::createSphere(
    std::shared_ptr<TraceableBuilder> b, const Vec3f& color, float am,
    float diff, float spec, float shi, const Point3f& center,
    const float& radius) {
  b->addTraceable(center, radius);
  b->addColor(color, am, diff, spec, shi);
  return b->getTraceable();
}

std::shared_ptr<Traceable> Scene::createTransformedSphere(
    std::shared_ptr<TraceableBuilder> b, const Mat4f& matrix,
    const Vec3f& color, float am, float diff, float spec, float shi,
    const Point3f& center, const float& radius) {
  b->addTraceable(center, radius);
  b->transformTraceable(matrix);
  b->addColor(color, am, diff, spec, shi);
  return b->getTraceable();
}
