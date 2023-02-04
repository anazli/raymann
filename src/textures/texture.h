#pragma once
#include <cmath>
#include <memory>

#include "textures/perlin.h"
#include "tools/vec3.h"

class Texture {
 public:
  virtual Vec3f value(float u, float v, const Vec3f &p) const = 0;
};

typedef std::shared_ptr<Texture> TexturePtr;

class ConstantTexture : public Texture {
 public:
  ConstantTexture() = default;
  ConstantTexture(const Vec3f &c) : m_color(c) {}
  Vec3f value(float u, float v, const Vec3f &p) const override {
    return m_color;
  };

 private:
  Vec3f m_color;
};

class CheckerTexture : public Texture {
 public:
  CheckerTexture() = default;
  CheckerTexture(TexturePtr t1, TexturePtr t2) : m_odd(t1), m_even(t2) {}
  Vec3f value(float u, float v, const Vec3f &p) const override {
    float sines = sin(10.f * p.x()) * sin(10.f * p.y()) * sin(10.f * p.z());
    if (sines < 0.f) return m_odd->value(u, v, p);
    return m_even->value(u, v, p);
  }

 private:
  TexturePtr m_odd;
  TexturePtr m_even;
};

class PerlinTexture : public Texture {
 public:
  PerlinTexture() = default;
  PerlinTexture(float scale) : m_scale(scale) {}
  Vec3f value(float u, float v, const Vec3f &p) const override {
    return Vec3f(1.f, 1.f, 1.f) * 0.5f *
           static_cast<float>(
               1.f + sin(m_scale * p.x() + 5.f * m_noise.turb(m_scale * p)));
  }

 private:
  Perlin m_noise;
  float m_scale;
};
