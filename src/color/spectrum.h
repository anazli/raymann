#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

template <int N>
class Spectrum {
 public:
  Spectrum(float value = 0.f) { m_samples = std::vector<float>(N, value); }

  const std::vector<float> &samples() const { return m_samples; }
  std::vector<float> &samples() { return m_samples; }

  Spectrum<N> operator-() const {
    Spectrum<N> ret(0.f);
    std::transform(m_samples.begin(), m_samples.end(), ret.samples().begin(),
                   [](const float &elem) { return -elem; });
    return ret;
  }
  Spectrum<N> &operator+=(const Spectrum<N> &other) {
    std::transform(m_samples.begin(), m_samples.end(), other.samples().begin(),
                   m_samples.begin(), [](float &elem1, const float &elem2) {
                     elem1 += elem2;
                     return elem1;
                   });
    return *this;
  }
  Spectrum<N> &operator-=(const Spectrum<N> &other) {
    *this += (-other);
    return *this;
  }
  Spectrum<N> &operator*=(const Spectrum<N> &other) {
    std::transform(m_samples.begin(), m_samples.end(), other.samples().begin(),
                   m_samples.begin(), [](float &elem1, const float &elem2) {
                     elem1 *= elem2;
                     return elem1;
                   });
    return *this;
  }
  Spectrum<N> &operator/=(const Spectrum<N> &other) {
    std::transform(m_samples.begin(), m_samples.end(), other.samples().begin(),
                   m_samples.begin(), [](float &elem1, const float &elem2) {
                     elem1 /= elem2;
                     return elem1;
                   });
    return *this;
  }

  Spectrum<N> clamp(float low = 0.f,
                    float high = std::numeric_limits<float>::infinity()) {
    std::transform(m_samples.begin(), m_samples.end(), m_samples.begin(),
                   [&low, &high](float &elem) {
                     elem = std::clamp(elem, low, high);
                     return elem;
                   });
    return *this;
  }

  bool operator==(const Spectrum<N> &other) const {
    return std::equal(m_samples.begin(), m_samples.end(),
                      other.samples().begin());
  }
  bool operator!=(const Spectrum<N> &other) const { return !(*this == other); }

  bool isBlack() const {
    return (std::find_if(m_samples.begin(), m_samples.end(),
                         [](const float &el) { return el != 0.f; }) ==
            m_samples.end());
  }
  bool hasNaNs() const {
    return (std::find_if(m_samples.begin(), m_samples.end(),
                         [](const float &el) { return std::isnan(el); }) !=
            m_samples.end());
  }

 protected:
  std::vector<float> m_samples;
};

template <int N>
Spectrum<N> operator+(const Spectrum<N> &l, const Spectrum<N> &r) {
  Spectrum<N> ret(0.f);
  std::transform(
      l.samples().begin(), l.samples().end(), r.samples().begin(),
      ret.samples().begin(),
      [](const float &elem1, const float &elem2) { return elem1 + elem2; });
  return ret;
}

template <int N>
Spectrum<N> operator-(const Spectrum<N> &l, const Spectrum<N> &r) {
  return l + (-r);
}

template <int N>
Spectrum<N> operator*(const Spectrum<N> &l, const Spectrum<N> &r) {
  Spectrum<N> ret(0.f);
  std::transform(
      l.samples().begin(), l.samples().end(), r.samples().begin(),
      ret.samples().begin(),
      [](const float &elem1, const float &elem2) { return elem1 * elem2; });
  return ret;
}

template <int N>
Spectrum<N> operator/(const Spectrum<N> &l, const Spectrum<N> &r) {
  Spectrum<N> ret(0.f);
  std::transform(
      l.samples().begin(), l.samples().end(), r.samples().begin(),
      ret.samples().begin(),
      [](const float &elem1, const float &elem2) { return elem1 / elem2; });
  return ret;
}

template <int N>
Spectrum<N> operator+(const Spectrum<N> &l, const float &f) {
  Spectrum<N> ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem + f; });
  return ret;
}

template <int N>
Spectrum<N> operator-(const Spectrum<N> &l, const float &f) {
  Spectrum<N> ret(0.f);
  std::transform(l.samples().begin(), l.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem - f; });
  return ret;
}

template <int N>
Spectrum<N> operator*(const Spectrum<N> &s, const float &f) {
  Spectrum<N> ret(0.f);
  std::transform(s.samples().begin(), s.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem * f; });
  return ret;
}

template <int N>
Spectrum<N> operator/(const Spectrum<N> &s, const float &f) {
  Spectrum<N> ret(0.f);
  std::transform(s.samples().begin(), s.samples().end(), ret.samples().begin(),
                 [&f](const float &elem) { return elem / f; });
  return ret;
}

template <int N>
Spectrum<N> sqrt(const Spectrum<N> &s) {
  Spectrum<N> ret(0.f);
  std::transform(ret.samples().begin(), ret.samples().end(),
                 s.samples().begin(), ret.samples().begin(),
                 [](float &elem1, const float &elem2) {
                   elem1 = static_cast<float>(std::sqrt(elem2));
                   return elem1;
                 });
  return ret;
}

template <int N>
Spectrum<N> exp(const Spectrum<N> &s) {
  Spectrum<N> ret(0.f);
  std::transform(ret.samples().begin(), ret.samples().end(),
                 s.samples().begin(), ret.samples().begin(),
                 [](float &elem1, const float &elem2) {
                   elem1 = static_cast<float>(std::exp(elem2));
                   return elem1;
                 });
  return ret;
}
template <int N>
Spectrum<N> pow(const Spectrum<N> &s, const int &p) {
  Spectrum<N> ret(0.f);
  std::transform(ret.samples().begin(), ret.samples().end(),
                 s.samples().begin(), ret.samples().begin(),
                 [&p](float &elem1, const float &elem2) {
                   elem1 = static_cast<float>(std::pow(elem2, p));
                   return elem1;
                 });
  return ret;
}
template <int N>
Spectrum<N> lerp(float t, const Spectrum<N> &l, const Spectrum<N> &r) {
  return (1.f - t) * l + t * r;
}
