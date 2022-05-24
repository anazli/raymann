#include "world/scene.h"

std::shared_ptr<Traceable> Scene::createSphere(std::shared_ptr<SphereBuilder> b,
                                               const Vec3f& color, float am,
                                               float diff, float spec,
                                               float shi, const Point3f& center,
                                               const float& radius) {
  b->addSphere(center, radius);
  b->addColor(color, am, diff, spec, shi);
  return b->getSphere();
}

std::shared_ptr<Traceable> Scene::createTransformedSphere(
    std::shared_ptr<SphereBuilder> b, const Mat4f& matrix, const Vec3f& color,
    float am, float diff, float spec, float shi, const Point3f& center,
    const float& radius) {
  b->addSphere(center, radius);
  b->transformSphere(matrix);
  b->addColor(color, am, diff, spec, shi);
  return b->getSphere();
}
