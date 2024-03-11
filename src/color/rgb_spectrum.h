#pragma once
#include "color/spectrum.h"
#include "tools/vec3.h"

class RGBSpectrum : public Spectrum<3> {
 public:
  RGBSpectrum(float value = 0.f);
  RGBSpectrum(const Spectrum<3> &other);

  Vec3f toRGB() const;
  Vec3f toXYZ() const;
  float y() const;
};

RGBSpectrum fromRGB(const Vec3f &v);
RGBSpectrum fromXYZ(const Vec3f &xyz);
Vec3f RGBToXYZ(const Vec3f &rgb);
Vec3f XYZToRGB(const Vec3f xyz);

using SpectrumX = RGBSpectrum;
