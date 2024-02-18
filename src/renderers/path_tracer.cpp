#include "renderers/path_tracer.h"

#include "composite/world.h"

void BruteForceMC::visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                                         const Ray &ray) {}

void BruteForceMC::visitSceneElementComposite(
    const SceneElementRawPtr elementComp, const Ray &ray) {
  if (elementComp->isWorld()) {
    m_out_color = computeColor(elementComp, ray);
  }
}

Vec3f BruteForceMC::computeColor(const SceneElementRawPtr world, const Ray &ray,
                                 int rec) {
  IntersectionRecord record;
  if (world->intersect(ray, record)) {
    Ray scattered;
    Vec3f attenuation;
    if (rec < 50 && record.object->getMaterial()->scatter(
                        ray, record, attenuation, scattered)) {
      return attenuation * computeColor(world, scattered, rec + 1);
    } else {
      return Vec3f(0.f, 0.f, 0.f);
    }
  }
  return m_background_color;
}
