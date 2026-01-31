#pragma once
#include <vec3.h>

#include <cmath>
#include <memory>

#include "perlin.h"

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
  ConstantTexture(const Vec3f &c);
  Vec3f value(float u, float v, const Vec3f &p) const override;
  void setColor(const Vec3f &col) override;

  static TexturePtr create(const Vec3f &color);

 private:
  Vec3f m_color;
};

class CheckerTexture : public Texture {
 public:
  ~CheckerTexture() override = default;
  CheckerTexture() = default;
  CheckerTexture(TexturePtr t1, TexturePtr t2);
  Vec3f value(float u, float v, const Vec3f &p) const override;
  void setColor(const Vec3f &col) override;

  static TexturePtr create(TexturePtr t1, TexturePtr t2);

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