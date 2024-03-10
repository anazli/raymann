#include "color/spectrum.h"

#include <algorithm>
#include <cmath>

Spectrum::Spectrum(float value, int samplesSize)
    : m_samplesSize(samplesSize) {
  m_samples = std::vector<float>(m_samplesSize, value);
}

const std::vector<float> &Spectrum::samples() const { return m_samples; }

std::vector<float> &Spectrum::samples() { return m_samples; }

Spectrum Spectrum::operator-() const {
  Spectrum ret(0, m_samplesSize);
  std::transform(m_samples.begin(), m_samples.end(), ret.samples().begin(),
                 [](const float &elem) { return -elem; });
  return ret;
}

Spectrum &Spectrum::operator+=(const Spectrum &other) {
  std::transform(m_samples.begin(), m_samples.end(), other.samples().begin(),
                 m_samples.begin(), [](float &elem1, const float &elem2) {
                   elem1 += elem2;
                   return elem1;
                 });
  return *this;
}

Spectrum &Spectrum::operator-=(const Spectrum &other) {
  *this += (-other);
  return *this;
}

Spectrum &Spectrum::operator*=(const Spectrum &other) {
  std::transform(m_samples.begin(), m_samples.end(), other.samples().begin(),
                 m_samples.begin(), [](float &elem1, const float &elem2) {
                   elem1 *= elem2;
                   return elem1;
                 });
  return *this;
}

Spectrum &Spectrum::operator/=(const Spectrum &other) {
  std::transform(m_samples.begin(), m_samples.end(), other.samples().begin(),
                 m_samples.begin(), [](float &elem1, const float &elem2) {
                   elem1 /= elem2;
                   return elem1;
                 });
  return *this;
}

Spectrum Spectrum::clamp(float low, float high) {
  std::transform(m_samples.begin(), m_samples.end(), m_samples.begin(),
                 [&low, &high](float &elem) {
                   elem = std::clamp(elem, low, high);
                   return elem;
                 });
  return *this;
}

bool Spectrum::operator==(const Spectrum &other) const {
  return std::equal(m_samples.begin(), m_samples.end(),
                    other.samples().begin());
}

bool Spectrum::operator!=(const Spectrum &other) const {
  return !(*this == other);
}

bool Spectrum::isBlack() const {
  return (std::find_if(m_samples.begin(), m_samples.end(), [](const float &el) {
            return el != 0.f;
          }) == m_samples.end());
}

bool Spectrum::hasNaNs() const {
  return (std::find_if(m_samples.begin(), m_samples.end(), [](const float &el) {
            return std::isnan(el);
          }) != m_samples.end());
}

Spectrum operator+(const Spectrum &l, const Spectrum &r) {
  Spectrum ret(0.f);
  std::transform(
      l.samples().begin(), l.samples().end(), r.samples().begin(),
      ret.samples().begin(),
      [](const float &elem1, const float &elem2) { return elem1 + elem2; });
  return ret;
}

Spectrum operator-(const Spectrum &l, const Spectrum &r) {
  return l + (-r);
}

Spectrum operator*(const Spectrum &l, const Spectrum &r) {
  Spectrum ret(0.f);
  std::transform(
      l.samples().begin(), l.samples().end(), r.samples().begin(),
      ret.samples().begin(),
      [](const float &elem1, const float &elem2) { return elem1 * elem2; });
  return ret;
}

Spectrum operator/(const Spectrum &l, const Spectrum &r) {
  Spectrum ret(0.f);
  std::transform(
      l.samples().begin(), l.samples().end(), r.samples().begin(),
      ret.samples().begin(),
      [](const float &elem1, const float &elem2) { return elem1 / elem2; });
  return ret;
}

Spectrum operator+(const Spectrum &l, const float &f) {
  Spectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem + f; });
  return ret;
}

Spectrum operator-(const Spectrum &l, const float &f) {
  Spectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem - f; });
  return ret;
}

Spectrum operator*(const Spectrum &l, const float &f) {
  Spectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem * f; });
  return ret;
}

Spectrum operator/(const Spectrum &l, const float &f) {
  Spectrum ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem / f; });
  return ret;
}

Spectrum sqrt(const Spectrum &s) {
  Spectrum ret(0.f);
  std::transform(ret.samples().begin(), ret.samples().end(),
                 s.samples().begin(), ret.samples().begin(),
                 [](float &elem1, const float &elem2) {
                   elem1 = static_cast<float>(std::sqrt(elem2));
                   return elem1;
                 });
  return ret;
}

Spectrum exp(const Spectrum &s) {
  Spectrum ret(0.f);
  std::transform(ret.samples().begin(), ret.samples().end(),
                 s.samples().begin(), ret.samples().begin(),
                 [](float &elem1, const float &elem2) {
                   elem1 = static_cast<float>(std::exp(elem2));
                   return elem1;
                 });
  return ret;
}

Spectrum pow(const Spectrum &s, const int &p) {
  Spectrum ret(0.f);
  std::transform(ret.samples().begin(), ret.samples().end(),
                 s.samples().begin(), ret.samples().begin(),
                 [&p](float &elem1, const float &elem2) {
                   elem1 = static_cast<float>(std::pow(elem2, p));
                   return elem1;
                 });
  return ret;
}

Spectrum lerp(float t, const Spectrum &l, const Spectrum &r) {
  return (1.f - t) * l + t * r;
}
