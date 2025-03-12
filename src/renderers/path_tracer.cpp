#include "renderers/path_tracer.h"

#include "stochastic/pdf.h"
#include "stochastic/stochastic_method.h"

PathTracer::PathTracer(std::unique_ptr<StochasticSampler> stMethod) {
  m_stochasticSampler = std::move(stMethod);
}

void PathTracer::visitSceneElement(const SceneElementRawPtr elementComp,
                                   const Ray &ray) {
  m_out_color = m_stochasticSampler->computeColor(this, elementComp);
}

Vec3D PathTracer::computeColor(const SceneElementRawPtr world, const Ray &ray,
                               int rec) {
  Intersection record;
  if (world->intersect(ray, record)) {
    Ray scattered;
    Vec3D attenuation;
    Vec3D emittedColor = record.primitive->getMaterial()->emmit();
    if (record.primitive->getMaterial()->isEmissive()) {
      return emittedColor;
    }
    if (rec > 0 && record.primitive->getMaterial()->scatter(
                       ray, record, attenuation, scattered)) {
      return emittedColor +
             attenuation * computeColor(world, scattered, rec - 1);
    }
  }
  return m_background_color;
}

void PathTracer::attachStochasticMethod(
    std::unique_ptr<StochasticSampler> stMethod) {
  m_stochasticSampler = std::move(stMethod);
}
