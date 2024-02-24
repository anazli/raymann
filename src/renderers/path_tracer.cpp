#include "renderers/path_tracer.h"

#include "composite/world.h"
#include "stochastic/stochastic_method.h"

PathTracer::PathTracer(std::unique_ptr<StochasticMethod> stMethod) {
  m_stochasticMethod = std::move(stMethod);
}

void PathTracer::visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                                       const Ray &ray) {}

void PathTracer::visitSceneElementComposite(
    const SceneElementRawPtr elementComp, const Ray &ray) {
  m_out_color = m_stochasticMethod->computeColor(this, elementComp);
}

Vec3f PathTracer::computeColor(const SceneElementRawPtr world, const Ray &ray,
                               int rec) {
  IntersectionRecord record;
  if (world->intersect(ray, record)) {
    Ray scattered;
    Vec3f attenuation;
    Vec3f emittedColor = record.object->getMaterial()->emmit();
    if (record.object->getMaterial()->isEmissive()) {
      return emittedColor;
    }
    if (rec > 0 && record.object->getMaterial()->scatter(
                       ray, record, attenuation, scattered)) {
      return emittedColor +
             attenuation * computeColor(world, scattered, rec - 1);
    }
  }
  return m_background_color;
}

void PathTracer::attachStochasticMethod(
    std::unique_ptr<StochasticMethod> stMethod) {
  m_stochasticMethod = std::move(stMethod);
}
