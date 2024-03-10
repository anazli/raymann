#pragma once

#include <limits>
#include <vector>

class Spectrum {
 public:
  Spectrum(float value = 0.f, int samplesSize = 60);

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

 protected:
  int m_samplesSize;
  std::vector<float> m_samples;
};

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
