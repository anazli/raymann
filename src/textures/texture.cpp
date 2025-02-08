#include "textures/texture.h"

ConstantTexture::ConstantTexture(const Vec3D &c) : m_color(c) {}

Vec3D ConstantTexture::value(float u, float v, const Vec3D &p) const {
  return m_color;
};

void ConstantTexture::setColor(const Vec3D &col) { m_color = col; }

TexturePtr ConstantTexture::create(const Vec3D &color) {
  return std::make_unique<ConstantTexture>(color);
}

CheckerTexture::CheckerTexture(TexturePtr t1, TexturePtr t2)
    : m_odd(std::move(t1)), m_even(std::move(t2)) {}

Vec3D CheckerTexture::value(float u, float v, const Vec3D &p) const {
  float sines = sin(10.f * p.x()) * sin(10.f * p.y()) * sin(10.f * p.z());
  if (sines < 0.f) return m_odd->value(u, v, p);
  return m_even->value(u, v, p);
}

void CheckerTexture::setColor(const Vec3D &col) {}

TexturePtr CheckerTexture::create(TexturePtr t1, TexturePtr t2) {
  return std::make_unique<CheckerTexture>(std::move(t1), std::move(t2));
}

TexturePtr PerlinTexture::create(float scale, const Vec3D &color) {
  return std::make_unique<PerlinTexture>(scale, color);
}

PerlinTexture::PerlinTexture(float scale, const Vec3D &color)
    : m_scale(scale), m_color(color) {}

Vec3D PerlinTexture::value(float u, float v, const Vec3D &p) const {
  return m_color * 0.5f *
         static_cast<float>(
             1.f + sin(m_scale * p.x() + 5.f * m_noise.turb(m_scale * p)));
}
void PerlinTexture::setColor(const Vec3D &col) {}
