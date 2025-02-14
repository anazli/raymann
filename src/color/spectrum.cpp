#include "color/spectrum.h"

#include <algorithm>
#include <cmath>

#include "application/error.h"

Spectrum::Spectrum(float value) { m_samples = std::vector<float>(3, value); }

const std::vector<float> &Spectrum::samples() const { return m_samples; }

std::vector<float> &Spectrum::samples() { return m_samples; }

Spectrum Spectrum::operator-() const {
  Spectrum ret(0);
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

Vec3D Spectrum::toRGB() const {
  return Vec3D(m_samples[0], m_samples[1], m_samples[2]);
}

Vec3D Spectrum::toXYZ() const { return RGBToXYZ(toRGB()); }

float Spectrum::y() const {
  const Vec3D YWeight(0.212671f, 0.715160f, 0.072169f);
  return YWeight[0] * m_samples[0] + YWeight[1] * m_samples[1] +
         YWeight[2] * m_samples[2];
}

Spectrum fromRGB(const Vec3D &v) {
  Spectrum s;
  s.samples()[0] = v.x();
  s.samples()[1] = v.y();
  s.samples()[2] = v.z();

  APP_ASSERT(!s.hasNaNs(), "RGB Spectrum has nans!");
  return s;
}

Spectrum fromXYZ(const Vec3D &xyz) {
  Spectrum s;
  Vec3D v = XYZToRGB(xyz);
  s.samples()[0] = v.x();
  s.samples()[1] = v.y();
  s.samples()[2] = v.z();
  return s;
}

Vec3D RGBToXYZ(const Vec3D &rgb) {
  Vec3D xyz;
  xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
  xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
  xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];
  return xyz;
}

Vec3D XYZToRGB(const Vec3D xyz) {
  Vec3D rgb;
  rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
  rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
  rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
  return rgb;
}

Spectrum operator+(const Spectrum &l, const Spectrum &r) {
  Spectrum ret(0.f);
  std::transform(
      l.samples().begin(), l.samples().end(), r.samples().begin(),
      ret.samples().begin(),
      [](const float &elem1, const float &elem2) { return elem1 + elem2; });
  return ret;
}

Spectrum operator-(const Spectrum &l, const Spectrum &r) { return l + (-r); }

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
