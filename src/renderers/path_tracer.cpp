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
  if (rec < 0) return Vec3D();
  Intersection record;
  if (!world->intersect(ray, record)) return m_background_color;

  Vec3D emittedColor = record.closest_scene_element->getMaterial()->emmit();

  Ray scattered;
  Vec3D attenuation;
  if (!record.closest_scene_element->getMaterial()->scatter(
          ray, record, attenuation, scattered)) {
    return emittedColor;
  }

  if (record.closest_scene_element->getMaterial()->getType() ==
          AppParameters::DIELECTRIC ||
      record.closest_scene_element->getMaterial()->getType() ==
          AppParameters::METAL) {
    return attenuation * computeColor(world, scattered, rec - 1);
  }

  StochasticPdfPtr lightPdf;
  if (m_diffuseLight) {
    lightPdf = std::make_shared<PrimitivePdf>(m_diffuseLight,
                                              record.getHitPoint(scattered));
  }
  CombinedPdf mixPdf(lightPdf,
                     record.closest_scene_element->getMaterial()->pdf(), 0.7f);
  auto rayFromPdf = Ray(record.getHitPoint(ray), mixPdf.generate());
  auto scatPdf = record.closest_scene_element->getMaterial()->scatteringPDF(
      scattered, record, rayFromPdf);
  auto pdf = mixPdf.value(rayFromPdf.direction());

  return emittedColor +
         attenuation * scatPdf * computeColor(world, rayFromPdf, rec - 1) / pdf;
}

void PathTracer::attachStochasticMethod(
    std::unique_ptr<StochasticSampler> stMethod) {
  m_stochasticSampler = std::move(stMethod);
}
