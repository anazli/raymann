#pragma once
#include <cmath>
#include <memory>

#include "textures/perlin.h"
#include "tools/vec3.h"

class Texture {
 public:
  virtual ~Texture() = default;
  virtual Vec3f value(float u, float v, const Vec3f &p) const = 0;
  virtual void setColor(const Vec3f &col) = 0;
};

using TexturePtr = std::unique_ptr<Texture>;
using TextureRawPtr = Texture *;

class ConstantTexture : public Texture {
 public:
  ~ConstantTexture() override = default;
  ConstantTexture() = default;
  ConstantTexture(const Vec3f &c) : m_color(c) {}
  Vec3f value(float u, float v, const Vec3f &p) const override {
    return m_color;
  };
  void setColor(const Vec3f &col) override { m_color = col; }

 private:
  Vec3f m_color;
};

class CheckerTexture : public Texture {
 public:
  ~CheckerTexture() override = default;
  CheckerTexture() = default;
  CheckerTexture(TexturePtr t1, TexturePtr t2)
      : m_odd(std::move(t1)), m_even(std::move(t2)) {}
  Vec3f value(float u, float v, const Vec3f &p) const override {
    float sines = sin(10.f * p.x()) * sin(10.f * p.y()) * sin(10.f * p.z());
    if (sines < 0.f) return m_odd->value(u, v, p);
    return m_even->value(u, v, p);
  }
  void setColor(const Vec3f &col) override {}

 private:
  TexturePtr m_odd;
  TexturePtr m_even;
};

class PerlinTexture : public Texture {
 public:
  ~PerlinTexture() override = default;
  PerlinTexture() = default;
  PerlinTexture(float scale, const Vec3f &color = Vec3f(1.f, 1.f, 1.f))
      : m_scale(scale), m_color(color) {}
  Vec3f value(float u, float v, const Vec3f &p) const override {
    return m_color * 0.5f *
           static_cast<float>(
               1.f + sin(m_scale * p.x() + 5.f * m_noise.turb(m_scale * p)));
  }
  void setColor(const Vec3f &col) override {}

 private:
  Perlin m_noise;
  float m_scale;
  Vec3f m_color;
};
