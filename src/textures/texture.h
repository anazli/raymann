#pragma once
#include <cmath>
#include <memory>

#include "color/spectrum.h"
#include "composite/intersection.h"
#include "tools/vec3.h"

class Texture {
 public:
  virtual ~Texture() = default;
  virtual Vec3D value(float u, float v, const Vec3D &p) const = 0;
  virtual Spectrum value(const Intersection &record) const {
    return Spectrum(0.f);
  }
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
  Spectrum value(const Intersection &record) const override;
  void setColor(const Vec3D &col) override;

  static TexturePtr create(const Vec3D &color);

 private:
  Vec3D m_color;
  Spectrum m_value;
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
