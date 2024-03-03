#include "renderers/path_tracer.h"

#include "composite/world.h"
#include "stochastic/pdf.h"
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
    if (rec > 0 && record.object->getMaterial()->scatter(
                       ray, record, attenuation, scattered)) {
      auto scatPdf =
          record.object->getMaterial()->scatteringPDF(ray, record, scattered);
      auto point = record.point(scattered);

      float pdf = 1.f;
      StochasticPdfPtr lightPdf;
      if (m_diffuseLight) {
        lightPdf = std::make_shared<PrimitivePdf>(m_diffuseLight, point);
      }
      StochasticPdfPtr cosPdf =
          std::make_shared<CosPdf>(record.object->normal(point));
      CombinedPdf mixPdf(lightPdf, cosPdf);
      scattered = Ray(point, mixPdf.generate());
      pdf = mixPdf.value(scattered.direction());
      return emittedColor + attenuation * scatPdf *
                                computeColor(world, scattered, rec - 1) / pdf;
    }
  }
  return m_background_color;
}

void PathTracer::attachStochasticMethod(
    std::unique_ptr<StochasticSampler> stMethod) {
  m_stochasticSampler = std::move(stMethod);
}
