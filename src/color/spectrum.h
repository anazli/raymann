#pragma once

#include <limits>
#include <vector>

#include "tools/vec3.h"

// RGB spectrum (might change to EM)
class Spectrum {
 public:
  Spectrum(float value = 0.f);

  const std::vector<float> &samples() const;
  std::vector<float> &samples();

  Spectrum operator-() const;
  Spectrum &operator+=(const Spectrum &other);
  Spectrum &operator-=(const Spectrum &other);
  Spectrum &operator*=(const Spectrum &other);
  Spectrum &operator/=(const Spectrum &other);

  Spectrum clamp(float low = 0.f,
                 float high = std::numeric_limits<float>::infinity());

  bool operator==(const Spectrum &other) const;
  bool operator!=(const Spectrum &other) const;

  bool isBlack() const;
  bool hasNaNs() const;

  Vec3D toRGB() const;
  Vec3D toXYZ() const;
  float y() const;

 protected:
  std::vector<float> m_samples;
};

Spectrum fromRGB(const Vec3D &v);
Spectrum fromXYZ(const Vec3D &xyz);
Vec3D RGBToXYZ(const Vec3D &rgb);
Vec3D XYZToRGB(const Vec3D xyz);

Spectrum operator+(const Spectrum &l, const Spectrum &r);
Spectrum operator-(const Spectrum &l, const Spectrum &r);
Spectrum operator*(const Spectrum &l, const Spectrum &r);
Spectrum operator/(const Spectrum &l, const Spectrum &r);

Spectrum operator+(const Spectrum &l, const float &f);
Spectrum operator-(const Spectrum &l, const float &f);
Spectrum operator*(const Spectrum &l, const float &f);
Spectrum operator/(const Spectrum &l, const float &f);

Spectrum sqrt(const Spectrum &s);
Spectrum exp(const Spectrum &s);
Spectrum pow(const Spectrum &s, const int &p);
Spectrum lerp(float t, const Spectrum &l, const Spectrum &r);
