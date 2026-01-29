// This code is based on or derived from the pbrt-v3 renderer
// (https://www.pbrt.org/). pbrt-v3 is distributed under the terms of the BSD
// 2-Clause License.

#pragma once

#include <vec3.h>

#include <limits>
#include <vector>

// RGB spectrum (might change to EM)
class Spectrum {
 public:
  explicit Spectrum(float value = 0.f);
  explicit Spectrum(const Vec3f &v);

  Vec3f samples() const;
  void setSamples(const Vec3f &s);

  Spectrum operator-() const;
  Spectrum &operator+=(const Spectrum &other);
  Spectrum &operator-=(const Spectrum &other);
  Spectrum &operator*=(const Spectrum &other);
  Spectrum &operator/=(const Spectrum &other);

  Spectrum clamp(float low = 0.f,
                 float high = std::numeric_limits<float>::infinity());

  auto operator<=>(const Spectrum &) const = default;

  bool isBlack() const;
  bool hasNaNs() const;

  Vec3f toRGB() const;
  Vec3f toXYZ() const;
  float y() const;

 private:
  Vec3f m_samples;
};

Spectrum fromRGB(const Vec3f &v);
Spectrum fromXYZ(const Vec3f &xyz);
Vec3f RGBToXYZ(const Vec3f &rgb);
Vec3f XYZToRGB(const Vec3f xyz);

Spectrum operator+(const Spectrum &l, const Spectrum &r);
Spectrum operator-(const Spectrum &l, const Spectrum &r);
Spectrum operator*(const Spectrum &l, const Spectrum &r);
Spectrum operator/(const Spectrum &l, const Spectrum &r);

Spectrum operator+(const Spectrum &l, float f);
Spectrum operator+(float f, const Spectrum &l);
Spectrum operator-(const Spectrum &l, float f);
Spectrum operator-(float f, const Spectrum &l);
Spectrum operator*(const Spectrum &l, float f);
Spectrum operator*(float f, const Spectrum &l);
Spectrum operator/(const Spectrum &l, float f);

Spectrum sqrt(const Spectrum &s);
Spectrum exp(const Spectrum &s);
Spectrum pow(const Spectrum &s, int p);
Spectrum lerp(float t, const Spectrum &l, const Spectrum &r);
