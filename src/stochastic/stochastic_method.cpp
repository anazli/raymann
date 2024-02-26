#include "stochastic/stochastic_method.h"

StochasticSampler::StochasticSampler(const BaseCameraPtr &cam, int pixelSamples,
                                     int materialDepth)
    : m_camera(cam),
      m_samplesPerPixel(pixelSamples),
      m_materialDepth(materialDepth) {
  rand = std::uniform_real_distribution<float>(0.f, 1.f);
  gen.seed(19844343409u);
}

float StochasticSampler::scatteringPDF(const Ray &r,
                                       const IntersectionRecord &record,
                                       const Ray &scatteredRay) const {
  return 1.f;
}

float StochasticSampler::pdf() const { return 1.f; }

float StochasticSampler::randomNumber() { return rand(gen); }

BruteForceSampler::BruteForceSampler(const BaseCameraPtr &cam, int pixelSamples,
                                     int materialDepth)
    : StochasticSampler(cam, pixelSamples, materialDepth) {}

Vec3f BruteForceSampler::computeColor(BaseRendererRawPtr renderer,
                                      const SceneElementRawPtr world) {
  if (world->isWorld() && renderer) {
    auto col = Vec3f();
    auto x = renderer->getPixelInfoX();
    auto y = renderer->getPixelInfoY();
    auto interval = m_camera->pixelSize() / 2.f;
    for (int s = 0; s < m_samplesPerPixel; ++s) {
      auto ax = x - interval;
      auto bx = x + interval;
      auto u = ax + randomNumber() * (bx - ax);

      auto ay = y - interval;
      auto by = y + interval;
      auto v = ay + randomNumber() * (by - ay);

      auto newRay = m_camera->getRay(u, v);
      col = col + renderer->computeColor(world, newRay, m_materialDepth);
    }
    col = col / static_cast<float>(m_samplesPerPixel);
    return Vec3f(static_cast<float>(sqrt(col.x())),
                 static_cast<float>(sqrt(col.y())),
                 static_cast<float>(sqrt(col.z())));
  }
  return Vec3f();
}

float BruteForceSampler::scatteringPDF(const Ray &r,
                                       const IntersectionRecord &record,
                                       const Ray &scatteredRay) const {
  auto cTheta = dot(record.object->normal(record.point(r)),
                    Vec3f(scatteredRay.direction()).normalize());
  return cTheta < 0 ? 0 : cTheta / PI;
}

float BruteForceSampler::pdf() const { return m_pdf; }

StratifiedSampler::StratifiedSampler(const BaseCameraPtr &cam, int pixelSamples,
                                     int materialDepth)
    : StochasticSampler(cam, pixelSamples, materialDepth) {}

Vec3f StratifiedSampler::computeColor(BaseRendererRawPtr renderer,
                                      const SceneElementRawPtr world) {
  if (world->isWorld() && renderer) {
    auto col = Vec3f();
    auto sqrtN = sqrt(m_samplesPerPixel);
    auto interval = m_camera->pixelSize() / sqrtN;
    for (int x = 0; x < static_cast<int>(sqrtN); ++x) {
      for (int y = 0; y < static_cast<int>(sqrtN); ++y) {
        auto ax = x - interval;
        auto bx = x + interval;
        auto u = ax + randomNumber() * (bx - ax);

        auto ay = y - interval;
        auto by = y + interval;
        auto v = ay + randomNumber() * (by - ay);

        auto newRay = m_camera->getRay(u, v);
        col = col + renderer->computeColor(world, newRay, m_materialDepth);
      }
    }
    col = col / static_cast<float>(m_samplesPerPixel);
    return Vec3f(static_cast<float>(sqrt(col.x())),
                 static_cast<float>(sqrt(col.y())),
                 static_cast<float>(sqrt(col.z())));
  }
  return Vec3f();
}
