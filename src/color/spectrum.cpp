#include "color/spectrum.h"

#include <algorithm>
#include <cmath>

CoeffSpectrum::CoeffSpectrum(float value, int samplesSize)
    : m_samplesSize(samplesSize) {
  m_samples = std::vector<float>(m_samplesSize, value);
}

const std::vector<float> &CoeffSpectrum::samples() const { return m_samples; }

std::vector<float> &CoeffSpectrum::samples() { return m_samples; }

CoeffSpectrum CoeffSpectrum::operator-() const {
  CoeffSpectrum ret(0, m_samplesSize);
  std::transform(m_samples.begin(), m_samples.end(), ret.samples().begin(),
                 [](const float &elem) { return -elem; });
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

CoeffSpectrum CoeffSpectrum::clamp(float low, float high) {
  std::transform(m_samples.begin(), m_samples.end(), m_samples.begin(),
                 [&low, &high](float &elem) {
                   elem = std::clamp(elem, low, high);
                   return elem;
                 });
  return *this;
}

bool CoeffSpectrum::operator==(const CoeffSpectrum &other) const {
  return std::equal(m_samples.begin(), m_samples.end(),
                    other.samples().begin());
}

bool CoeffSpectrum::operator!=(const CoeffSpectrum &other) const {
  return !(*this == other);
}

bool CoeffSpectrum::isBlack() const {
  return (std::find_if(m_samples.begin(), m_samples.end(), [](const float &el) {
            return el != 0.f;
          }) == m_samples.end());
}

bool CoeffSpectrum::hasNaNs() const {
  return (std::find_if(m_samples.begin(), m_samples.end(), [](const float &el) {
            return std::isnan(el);
          }) != m_samples.end());
}

CoeffSpectrum operator+(const CoeffSpectrum &l, const CoeffSpectrum &r) {
  CoeffSpectrum ret(0.f);
  std::transform(
      l.samples().begin(), l.samples().end(), r.samples().begin(),
      ret.samples().begin(),
      [](const float &elem1, const float &elem2) { return elem1 + elem2; });
  return ret;
}

CoeffSpectrum operator-(const CoeffSpectrum &l, const CoeffSpectrum &r) {
  return l + (-r);
}

CoeffSpectrum operator*(const CoeffSpectrum &l, const CoeffSpectrum &r) {
  CoeffSpectrum ret(0.f);
  std::transform(
      l.samples().begin(), l.samples().end(), r.samples().begin(),
      ret.samples().begin(),
      [](const float &elem1, const float &elem2) { return elem1 * elem2; });
  return ret;
}

CoeffSpectrum operator/(const CoeffSpectrum &l, const CoeffSpectrum &r) {
  CoeffSpectrum ret(0.f);
  std::transform(
      l.samples().begin(), l.samples().end(), r.samples().begin(),
      ret.samples().begin(),
      [](const float &elem1, const float &elem2) { return elem1 / elem2; });
  return ret;
}

CoeffSpectrum operator+(const CoeffSpectrum &l, const float &f) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem + f; });
  return ret;
}

CoeffSpectrum operator-(const CoeffSpectrum &l, const float &f) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem - f; });
  return ret;
}

CoeffSpectrum operator*(const CoeffSpectrum &l, const float &f) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem * f; });
  return ret;
}

CoeffSpectrum operator/(const CoeffSpectrum &l, const float &f) {
  CoeffSpectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem / f; });
  return ret;
}

CoeffSpectrum sqrt(const CoeffSpectrum &s) {
  CoeffSpectrum ret(0.f);
  std::transform(ret.samples().begin(), ret.samples().end(),
                 s.samples().begin(), ret.samples().begin(),
                 [](float &elem1, const float &elem2) {
                   elem1 = static_cast<float>(std::sqrt(elem2));
                   return elem1;
                 });
  return ret;
}

CoeffSpectrum exp(const CoeffSpectrum &s) {
  CoeffSpectrum ret(0.f);
  std::transform(ret.samples().begin(), ret.samples().end(),
                 s.samples().begin(), ret.samples().begin(),
                 [](float &elem1, const float &elem2) {
                   elem1 = static_cast<float>(std::exp(elem2));
                   return elem1;
                 });
  return ret;
}

CoeffSpectrum pow(const CoeffSpectrum &s, const int &p) {
  CoeffSpectrum ret(0.f);
  std::transform(ret.samples().begin(), ret.samples().end(),
                 s.samples().begin(), ret.samples().begin(),
                 [&p](float &elem1, const float &elem2) {
                   elem1 = static_cast<float>(std::pow(elem2, p));
                   return elem1;
                 });
  return ret;
}
