#pragma once

#include <vector>

class CoeffSpectrum {
 public:
  CoeffSpectrum(float value, int samplesSize = 60);

  const std::vector<float> &samples() const;
  std::vector<float> &samples();

  CoeffSpectrum operator-() const;
  CoeffSpectrum &operator+=(const CoeffSpectrum &other);
  CoeffSpectrum &operator-=(const CoeffSpectrum &other);
  CoeffSpectrum &operator*=(const CoeffSpectrum &other);
  CoeffSpectrum &operator/=(const CoeffSpectrum &other);

  bool operator==(const CoeffSpectrum &other);
  bool operator!=(const CoeffSpectrum &other);

  bool isBlack() const;

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
