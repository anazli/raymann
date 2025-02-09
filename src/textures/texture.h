#pragma once
#include <cmath>
#include <memory>

#include "textures/perlin.h"
#include "tools/vec3.h"

class Texture {
 public:
  virtual ~Texture() = default;
  virtual Vec3D value(float u, float v, const Vec3D &p) const = 0;
  virtual void setColor(const Vec3D &col) = 0;
};

using TexturePtr = std::unique_ptr<Texture>;
using TextureRawPtr = Texture *;

class ConstantTexture : public Texture {
 public:
  ~ConstantTexture() override = default;
  ConstantTexture() = default;
  ConstantTexture(const Vec3D &c);
  Vec3D value(float u, float v, const Vec3D &p) const override;
  void setColor(const Vec3D &col) override;

  static TexturePtr create(const Vec3D &color);

 private:
  Vec3D m_color;
};

class CheckerTexture : public Texture {
 public:
  ~CheckerTexture() override = default;
  CheckerTexture() = default;
  CheckerTexture(TexturePtr t1, TexturePtr t2);
  Vec3D value(float u, float v, const Vec3D &p) const override;
  void setColor(const Vec3D &col) override;

  static TexturePtr create(TexturePtr t1, TexturePtr t2);

 private:
  TexturePtr m_odd;
  TexturePtr m_even;
};

class PerlinTexture : public Texture {
 public:
  ~PerlinTexture() override = default;
  PerlinTexture(float scale, const Vec3D &color);
  Vec3D value(float u, float v, const Vec3D &p) const override;
  void setColor(const Vec3D &col) override;

  static TexturePtr create(float scale, const Vec3D &color);

 private:
  Perlin m_noise;
  float m_scale;
  Vec3D m_color;
};
