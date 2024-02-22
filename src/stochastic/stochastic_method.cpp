#include "stochastic/stochastic_method.h"

StochasticMethod::StochasticMethod(const BaseCameraPtr &cam, int pixelSamples,
                                   int materialDepth)
    : m_camera(cam),
      m_samplesPerPixel(pixelSamples),
      m_materialDepth(materialDepth) {
  rand = std::uniform_real_distribution<float>(0.f, 1.f);
  gen.seed(19844343409u);
}

float StochasticMethod::randomNumber() { return rand(gen); }

BruteForceMC::BruteForceMC(const BaseCameraPtr &cam, int pixelSamples,
                           int materialDepth)
    : StochasticMethod(cam, pixelSamples, materialDepth) {}

Vec3f BruteForceMC::computeColor(BaseRendererRawPtr renderer,
                                 const SceneElementRawPtr world) {
  if (world->isWorld() && renderer) {
    auto col = Vec3f();
    auto x = renderer->getPixelInfoX();
    auto y = renderer->getPixelInfoY();
    for (int s = 0; s < m_samplesPerPixel; ++s) {
      auto ax = x - m_camera->pixelSize() / 2.f;
      auto bx = x + m_camera->pixelSize() / 2.f;
      auto u = ax + randomNumber() * (bx - ax);

      auto ay = y - m_camera->pixelSize() / 2.f;
      auto by = y + m_camera->pixelSize() / 2.f;
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
