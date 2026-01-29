#pragma once
#include <vec3.h>

#include <cmath>
#include <memory>

#include "color/spectrum.h"
#include "composite/intersection.h"

class Texture {
 public:
  virtual ~Texture() = default;
  virtual Vec3f value(float u, float v, const Vec3f &p) const = 0;
  virtual Spectrum value(const Intersection &record) const {
    return Spectrum(0.f);
  }
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
  Spectrum value(const Intersection &record) const override;
  void setColor(const Vec3f &col) override;

  static TexturePtr create(const Vec3f &color);

 private:
  Vec3f m_color;
  Spectrum m_value;
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
