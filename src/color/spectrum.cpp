#include "color/spectrum.h"

#include <algorithm>
#include <cmath>

#include "application/error.h"
#include "spectrum.h"

Spectrum::Spectrum(float value) { m_samples = Vec3D(value, value, value); }
Spectrum::Spectrum(const Vec3D &v) : m_samples(v) {}

Vec3D Spectrum::samples() const { return m_samples; }

Spectrum Spectrum::operator-() const { return Spectrum(-m_samples); }

Spectrum &Spectrum::operator+=(const Spectrum &other) {
  (*this).m_samples = (*this).m_samples + other.m_samples;
  return *this;
}

Spectrum &Spectrum::operator-=(const Spectrum &other) {
  *this += (-other);
  return *this;
}

Spectrum &Spectrum::operator*=(const Spectrum &other) {
  (*this).m_samples = (*this).m_samples * other.m_samples;
  return *this;
}

Spectrum &Spectrum::operator/=(const Spectrum &other) {
  (*this).m_samples = (*this).m_samples / other.m_samples;
  return *this;
}

Spectrum Spectrum::clamp(float low, float high) {
  return Spectrum(Vec3D(std::clamp(m_samples.x(), low, high),
                        std::clamp(m_samples.y(), low, high),
                        std::clamp(m_samples.z(), low, high)));
}

bool Spectrum::isBlack() const { return m_samples == Vec3D(); }

bool Spectrum::hasNaNs() const {
  return std::isnan(m_samples.x()) && std::isnan(m_samples.y()) &&
         std::isnan(m_samples.z());
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
  return Spectrum(l.samples() + r.samples());
}

Spectrum operator-(const Spectrum &l, const Spectrum &r) { return l + (-r); }

Spectrum operator*(const Spectrum &l, const Spectrum &r) {
  return Spectrum(l.samples() * r.samples());
}

Spectrum operator/(const Spectrum &l, const Spectrum &r) {
  return Spectrum(l.samples() / r.samples());
}

Spectrum operator+(const Spectrum &l, float f) {
  return Spectrum(l.samples() + f);
}

Spectrum operator+(float f, const Spectrum &l) { return l + f; }

Spectrum operator-(const Spectrum &l, float f) { return l + (-f); }

Spectrum operator-(float f, const Spectrum &l) { return l - f; }

Spectrum operator*(const Spectrum &l, float f) {
  return Spectrum(l.samples() * f);
}

Spectrum operator*(float f, const Spectrum &l) { return l * f; }

Spectrum operator/(const Spectrum &l, float f) {
  return Spectrum(l.samples() / f);
}

Spectrum sqrt(const Spectrum &s) {
  auto v = s.samples();
  return Spectrum(Vec3D(sqrt(v.x()), sqrt(v.y()), sqrt(v.z())));
}

Spectrum exp(const Spectrum &s) {
  auto v = s.samples();
  return Spectrum(Vec3D(std::exp(v.x()), std::exp(v.y()), std::exp(v.z())));
}

Spectrum pow(const Spectrum &s, int p) {
  auto v = s.samples();
  return Spectrum(Vec3D(pow(v.x(), p), pow(v.y(), p), pow(v.z(), p)));
}

Spectrum lerp(float t, const Spectrum &l, const Spectrum &r) {
  return (1.f - t) * l + t * r;
}
