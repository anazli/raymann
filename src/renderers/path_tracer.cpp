#include "renderers/path_tracer.h"

#include "composite/world.h"
#include "stochastic/stochastic_method.h"

PathTracer::PathTracer(std::unique_ptr<StochasticSampler> stMethod) {
  m_stochasticSampler = std::move(stMethod);
}

void PathTracer::visitSceneElementLeaf(const SceneElementRawPtr elementLeaf,
                                       const Ray &ray) {}

void PathTracer::visitSceneElementComposite(
    const SceneElementRawPtr elementComp, const Ray &ray) {
  m_out_color = m_stochasticSampler->computeColor(this, elementComp);
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
    if (rec > 0 &&
        record.object->getMaterial()->scatter(ray, record, attenuation,
                                              scattered, m_stochasticSampler)) {
      // auto scPdf = m_stochasticSampler->scatteringPDF(ray, record,
      // scattered);
      return emittedColor + attenuation * /*scPdf **/
                                computeColor(world, scattered, rec - 1);  // /
      // m_stochasticMethod->pdf();
    }
  }
  return m_background_color;
}

void PathTracer::attachStochasticMethod(
    std::unique_ptr<StochasticSampler> stMethod) {
  m_stochasticSampler = std::move(stMethod);
}
