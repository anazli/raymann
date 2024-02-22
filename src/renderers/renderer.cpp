#include "renderers/renderer.h"

#include "stochastic/stochastic_method.h"

void BaseRenderer::attachStochasticMethod(
    std::unique_ptr<StochasticMethod> stMethod) {}

Vec3f BaseRenderer::getColor() const { return m_out_color; }

void BaseRenderer::setPixelInfo(const int &x, const int y) {
  m_x = static_cast<float>(x);
  m_y = static_cast<float>(y);
}

float BaseRenderer::getPixelInfoX() const { return m_x; }

float BaseRenderer::getPixelInfoY() const { return m_y; }

void BaseRenderer::setBackgroundColor(const Vec3f &color) {
  m_background_color = color;
}

Vec3f BaseRenderer::backgroundColor() const { return m_background_color; }
