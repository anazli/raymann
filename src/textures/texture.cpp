#include "textures/texture.h"

#include "texture.h"

ConstantTexture::ConstantTexture(const Vec3f &c) : m_color(c), m_value(c) {}

Vec3f ConstantTexture::value(float u, float v, const Vec3f &p) const {
  return m_color;
};

void ConstantTexture::setColor(const Vec3f &col) { m_color = col; }

TexturePtr ConstantTexture::create(const Vec3f &color) {
  return std::make_unique<ConstantTexture>(color);
}

CheckerTexture::CheckerTexture(TexturePtr t1, TexturePtr t2)
    : m_odd(std::move(t1)), m_even(std::move(t2)) {}

Vec3f CheckerTexture::value(float u, float v, const Vec3f &p) const {
  float sines = sin(10.f * p.x()) * sin(10.f * p.y()) * sin(10.f * p.z());
  if (sines < 0.f) return m_odd->value(u, v, p);
  return m_even->value(u, v, p);
}

void CheckerTexture::setColor(const Vec3f &col) {}

TexturePtr CheckerTexture::create(TexturePtr t1, TexturePtr t2) {
  return std::make_unique<CheckerTexture>(std::move(t1), std::move(t2));
}
