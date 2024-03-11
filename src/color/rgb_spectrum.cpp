#include "color/rgb_spectrum.h"

#include "application/error.h"

RGBSpectrum::RGBSpectrum(float value) : Spectrum(value) {}

RGBSpectrum::RGBSpectrum(const Spectrum<3> &other) : Spectrum<3>(other) {}

Vec3f RGBSpectrum::toRGB() const {
  return Vec3f(m_samples[0], m_samples[1], m_samples[2]);
}

Vec3f RGBSpectrum::toXYZ() const { return RGBToXYZ(toRGB()); }

float RGBSpectrum::y() const {
  const Vec3f YWeight(0.212671f, 0.715160f, 0.072169f);
  return YWeight[0] * m_samples[0] + YWeight[1] * m_samples[1] +
         YWeight[2] * m_samples[2];
}

RGBSpectrum fromRGB(const Vec3f &v) {
  RGBSpectrum s;
  s.samples()[0] = v.x();
  s.samples()[1] = v.y();
  s.samples()[2] = v.z();

  APP_ASSERT(!s.hasNaNs(), "RGB Spectrum has nans!");
  return s;
}

RGBSpectrum fromXYZ(const Vec3f &xyz) {
  RGBSpectrum s;
  Vec3f v = XYZToRGB(xyz);
  s.samples()[0] = v.x();
  s.samples()[1] = v.y();
  s.samples()[2] = v.z();
  return s;
}

Vec3f RGBToXYZ(const Vec3f &rgb) {
  Vec3f xyz;
  xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
  xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
  xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];
  return xyz;
}

Vec3f XYZToRGB(const Vec3f xyz) {
  Vec3f rgb;
  rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
  rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
  rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
  return rgb;
}
