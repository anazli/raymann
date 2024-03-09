#include "color/spectrum.h"

#include <algorithm>

CoeffSpectrum::CoeffSpectrum(float value, int samplesSize)
    : m_samplesSize(samplesSize) {
  m_samples = std::vector<float>(m_samplesSize, value);
}

const std::vector<float> &CoeffSpectrum::samples() const { return m_samples; }

std::vector<float> &CoeffSpectrum::samples() { return m_samples; }

CoeffSpectrum CoeffSpectrum::operator-() const {
  CoeffSpectrum ret(0, m_samplesSize);
  std::transform(m_samples.begin(), m_samples.end(), ret.samples().begin(),
                 [](float &elem) { return -elem; });
  return ret;
}

CoeffSpectrum &CoeffSpectrum::operator+=(const CoeffSpectrum &other) {
  std::transform(m_samples.begin(), m_samples.end(), other.samples().begin(),
                 m_samples.begin(), [](float &elem1, const float &elem2) {
                   elem1 += elem2;
                   return elem1;
                 });
  return *this;
}

CoeffSpectrum &CoeffSpectrum::operator-=(const CoeffSpectrum &other) {
  *this += (-other);
  return *this;
}

CoeffSpectrum &CoeffSpectrum::operator*=(const CoeffSpectrum &other) {
  std::transform(m_samples.begin(), m_samples.end(), other.samples().begin(),
                 m_samples.begin(), [](float &elem1, const float &elem2) {
                   elem1 *= elem2;
                   return elem1;
                 });
  return *this;
}

CoeffSpectrum &CoeffSpectrum::operator/=(const CoeffSpectrum &other) {
  std::transform(m_samples.begin(), m_samples.end(), other.samples().begin(),
                 m_samples.begin(), [](float &elem1, const float &elem2) {
                   elem1 /= elem2;
                   return elem1;
                 });
  return *this;
}

bool CoeffSpectrum::operator==(const CoeffSpectrum &other) {
  return std::equal(m_samples.begin(), m_samples.end(),
                    other.samples().begin());
}

bool CoeffSpectrum::operator!=(const CoeffSpectrum &other) {
  return !(*this == other);
}

bool CoeffSpectrum::isBlack() const {
  return (std::find_if(m_samples.begin(), m_samples.end(), [](const float &el) {
            return el == 0.f;
          }) != m_samples.end());
}

CoeffSpectrum operator+(const CoeffSpectrum &l, const CoeffSpectrum &r) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), r.samples().begin(),
                 ret.samples().begin(), [](float &elem1, const float &elem2) {
                   return elem1 + elem2;
                 });
  return ret;
}

CoeffSpectrum operator-(const CoeffSpectrum &l, const CoeffSpectrum &r) {
  return l + (-r);
}

CoeffSpectrum operator*(const CoeffSpectrum &l, const CoeffSpectrum &r) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), r.samples().begin(),
                 ret.samples().begin(), [](float &elem1, const float &elem2) {
                   return elem1 * elem2;
                 });
  return ret;
}

CoeffSpectrum operator/(const CoeffSpectrum &l, const CoeffSpectrum &r) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), r.samples().begin(),
                 ret.samples().begin(), [](float &elem1, const float &elem2) {
                   return elem1 / elem2;
                 });
  return ret;
}

CoeffSpectrum operator+(const CoeffSpectrum &l, const float &f) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](float &elem) { return elem + f; });
  return ret;
}

CoeffSpectrum operator-(const CoeffSpectrum &l, const float &f) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](float &elem) { return elem - f; });
  return ret;
}

CoeffSpectrum operator*(const CoeffSpectrum &l, const float &f) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](float &elem) { return elem * f; });
  return ret;
}

CoeffSpectrum operator/(const CoeffSpectrum &l, const float &f) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](float &elem) { return elem / f; });
  return ret;
}
