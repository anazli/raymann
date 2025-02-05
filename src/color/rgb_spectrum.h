#pragma once
#include "color/spectrum.h"
#include "tools/vec3.h"

class RGBSpectrum : public Spectrum {
 public:
  RGBSpectrum(float value = 0.f);

  Vec3D toRGB() const;
  Vec3D toXYZ() const;
  float y() const;
};

RGBSpectrum fromRGB(const Vec3D &v);
RGBSpectrum fromXYZ(const Vec3D &xyz);
Vec3D RGBToXYZ(const Vec3D &rgb);
Vec3D XYZToRGB(const Vec3D xyz);
