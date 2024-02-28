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

void StochasticSampler::setPdf(float val) { m_pdf = val; }

float StochasticSampler::randomNumber() { return rand(gen); }

float StochasticSampler::randomNumber(float a, float b) {
  std::uniform_real_distribution<float> rnd(a, b);
  return rnd(gen);
}

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
  // auto cTheta = dot(record.object->normal(record.point(r)),
  //                   Vec3f(scatteredRay.direction()).normalize());
  // return cTheta < 0 ? 0 : cTheta / PI;
  return 1.f / (2.f * PI);
}

float BruteForceSampler::pdf() const { return m_pdf; }

StratifiedSampler::StratifiedSampler(const BaseCameraPtr &cam, int pixelSamples,
                                     int materialDepth)
    : StochasticSampler(cam, pixelSamples, materialDepth) {}

Vec3f StratifiedSampler::computeColor(BaseRendererRawPtr renderer,
                                      const SceneElementRawPtr world) {
  if (world->isWorld() && renderer) {
    auto col = Vec3f();
    auto x = renderer->getPixelInfoX();
    auto y = renderer->getPixelInfoY();
    auto sqrtN = floor(sqrt(m_samplesPerPixel));
    auto stratumInterval = m_camera->pixelSize() / 2.f;
    auto strataInterval = m_camera->pixelSize() / sqrtN;
    for (int i = 0; i < sqrtN; ++i) {
      for (int j = 0; j < sqrtN; ++j) {
        auto ax = x - stratumInterval + i * strataInterval;
        auto bx = ax + strataInterval;
        auto u = ax + randomNumber() * (bx - ax);

        auto ay = y - stratumInterval + j * strataInterval;
        auto by = ay + strataInterval;
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
