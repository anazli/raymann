#include "renderers/renderer.h"

#include "stochastic/stochastic_method.h"

void BaseRenderer::attachStochasticMethod(
    std::unique_ptr<StochasticSampler> stMethod) {}

Vec3D BaseRenderer::getColor() const { return m_out_color; }

void BaseRenderer::setPixelInfo(const int &x, const int y) {
  m_x = static_cast<float>(x);
  m_y = static_cast<float>(y);
}

float BaseRenderer::getPixelInfoX() const { return m_x; }

float BaseRenderer::getPixelInfoY() const { return m_y; }

void BaseRenderer::setBackgroundColor(const Vec3D &color) {
  m_background_color = color;
}

Vec3D BaseRenderer::backgroundColor() const { return m_background_color; }

void BaseRenderer::addDiffuseLight(SceneElementRawPtr light) {
  m_diffuseLight = light;
}

SceneElementRawPtr BaseRenderer::getDiffuseLight() const {
  return m_diffuseLight;
}
