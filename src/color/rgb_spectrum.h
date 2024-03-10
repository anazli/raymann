#pragma once
#include "color/spectrum.h"
#include "tools/vec3.h"

class RGBSpectrum : public Spectrum {
 public:
  RGBSpectrum(float value = 0.f);

  Vec3f toRGB() const;
  Vec3f toXYZ() const;
  float y() const;
};

RGBSpectrum fromRGB(const Vec3f &v);
RGBSpectrum fromXYZ(const Vec3f &xyz);
Vec3f RGBToXYZ(const Vec3f &rgb);
Vec3f XYZToRGB(const Vec3f xyz);
