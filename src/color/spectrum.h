#pragma once

#include <limits>
#include <vector>

class CoeffSpectrum {
 public:
  CoeffSpectrum(float value = 0.f, int samplesSize = 60);

  const std::vector<float> &samples() const;
  std::vector<float> &samples();

  CoeffSpectrum operator-() const;
  CoeffSpectrum &operator+=(const CoeffSpectrum &other);
  CoeffSpectrum &operator-=(const CoeffSpectrum &other);
  CoeffSpectrum &operator*=(const CoeffSpectrum &other);
  CoeffSpectrum &operator/=(const CoeffSpectrum &other);

  CoeffSpectrum clamp(float low = 0.f,
                      float high = std::numeric_limits<float>::infinity());

  bool operator==(const CoeffSpectrum &other) const;
  bool operator!=(const CoeffSpectrum &other) const;

  bool isBlack() const;
  bool hasNaNs() const;

 private:
  int m_samplesSize;
  std::vector<float> m_samples;
};

CoeffSpectrum operator+(const CoeffSpectrum &l, const CoeffSpectrum &r);
CoeffSpectrum operator-(const CoeffSpectrum &l, const CoeffSpectrum &r);
CoeffSpectrum operator*(const CoeffSpectrum &l, const CoeffSpectrum &r);
CoeffSpectrum operator/(const CoeffSpectrum &l, const CoeffSpectrum &r);

CoeffSpectrum operator+(const CoeffSpectrum &l, const float &f);
CoeffSpectrum operator-(const CoeffSpectrum &l, const float &f);
CoeffSpectrum operator*(const CoeffSpectrum &l, const float &f);
CoeffSpectrum operator/(const CoeffSpectrum &l, const float &f);

CoeffSpectrum sqrt(const CoeffSpectrum &s);
CoeffSpectrum exp(const CoeffSpectrum &s);
CoeffSpectrum pow(const CoeffSpectrum &s, const int &p);
CoeffSpectrum lerp(float t, const CoeffSpectrum &l, const CoeffSpectrum &r);
