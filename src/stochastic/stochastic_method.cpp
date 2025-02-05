#include "stochastic/stochastic_method.h"

#include "stochastic/random.h"

StochasticSampler::StochasticSampler(const BaseCameraPtr &cam, int pixelSamples,
                                     int materialDepth)
    : m_camera(cam),
      m_samplesPerPixel(pixelSamples),
      m_materialDepth(materialDepth) {}

Vec3D StochasticSampler::colorCorrection(Vec3D &color) {
  color = color / static_cast<float>(m_samplesPerPixel);
  return Vec3D(static_cast<float>(sqrt(color.x())),
               static_cast<float>(sqrt(color.y())),
               static_cast<float>(sqrt(color.z())));
}

void StochasticSampler::addRandomSample(Vec3D &outputColor,
                                        const Vec3D &randomSampleColor) {
  Vec3D tempColor(randomSampleColor);
  if (tempColor.x() != tempColor.x()) tempColor.setX(0.f);
  if (tempColor.y() != tempColor.y()) tempColor.setY(0.f);
  if (tempColor.z() != tempColor.z()) tempColor.setZ(0.f);
  outputColor = outputColor + tempColor;
}

BruteForceSampler::BruteForceSampler(const BaseCameraPtr &cam, int pixelSamples,
                                     int materialDepth)
    : StochasticSampler(cam, pixelSamples, materialDepth) {}

Vec3D BruteForceSampler::computeColor(BaseRendererRawPtr renderer,
                                      const SceneElementRawPtr world) {
  if (world->isWorld() && renderer) {
    auto col = Vec3D();
    auto x = renderer->getPixelInfoX();
    auto y = renderer->getPixelInfoY();
    auto interval = m_camera->pixelSize() / 2.f;
    for (int s = 0; s < m_samplesPerPixel; ++s) {
      auto ax = x - interval;
      auto bx = x + interval;
      auto u = ax + Random::randomNumber() * (bx - ax);

      auto ay = y - interval;
      auto by = y + interval;
      auto v = ay + Random::randomNumber() * (by - ay);

      auto newRay = m_camera->getRay(u, v);
      Vec3D sColor = renderer->computeColor(world, newRay, m_materialDepth);
      addRandomSample(col, sColor);
    }
    return colorCorrection(col);
  }
  return Vec3D();
}

StratifiedSampler::StratifiedSampler(const BaseCameraPtr &cam, int pixelSamples,
                                     int materialDepth)
    : StochasticSampler(cam, pixelSamples, materialDepth) {}

Vec3D StratifiedSampler::computeColor(BaseRendererRawPtr renderer,
                                      const SceneElementRawPtr world) {
  if (world->isWorld() && renderer) {
    auto col = Vec3D();
    auto x = renderer->getPixelInfoX();
    auto y = renderer->getPixelInfoY();
    auto sqrtN = floor(sqrt(m_samplesPerPixel));
    auto stratumInterval = m_camera->pixelSize() / 2.f;
    auto strataInterval = m_camera->pixelSize() / sqrtN;
    for (int i = 0; i < sqrtN; ++i) {
      for (int j = 0; j < sqrtN; ++j) {
        auto ax = x - stratumInterval + i * strataInterval;
        auto bx = ax + strataInterval;
        auto u = ax + Random::randomNumber() * (bx - ax);

        auto ay = y - stratumInterval + j * strataInterval;
        auto by = ay + strataInterval;
        auto v = ay + Random::randomNumber() * (by - ay);

        auto newRay = m_camera->getRay(u, v);
        Vec3D sColor = renderer->computeColor(world, newRay, m_materialDepth);
        addRandomSample(col, sColor);
      }
    }
    return colorCorrection(col);
  }
  return Vec3D();
}
