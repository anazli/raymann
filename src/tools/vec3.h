#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

#include "vec4.h"

template <class T>
class Vec3 {
 public:
  Vec3() = default;
  Vec3(T p1, T p2, T p3) : m_x{p1}, m_y{p2}, m_z{p3} {}

  Vec3(const Vec3<T>& other) : m_x{other.m_x}, m_y{other.m_y}, m_z{other.m_z} {}
  Vec3(const Vec4<T>& v) : m_x{v.x()}, m_y{v.y()}, m_z{v.z()} {}

  T x() const { return m_x; }
  T y() const { return m_y; }
  T z() const { return m_z; }

  void setX(T num) { m_x = num; }
  void setY(T num) { m_y = num; }
  void setZ(T num) { m_z = num; }

  void setXYZ(T num) { m_x = m_y = m_z = num; }

  void setXYZ(T num1, T num2, T num3) {
    m_x = num1;
    m_y = num2;
    m_z = num3;
  }

  T operator[](int i) const {
    assert(i >= 0 && i <= 2);
    if (i == 0) return m_x;
    if (i == 1) return m_y;
    return m_z;
  }

  T& operator[](int i) {
    assert(i >= 0 && i <= 2);
    if (i == 0) return m_x;
    if (i == 1) return m_y;
    return m_z;
  }

  Vec3<T>& operator=(const Vec3<T>& rhs) {
    m_x = rhs.m_x;
    m_y = rhs.m_y;
    m_z = rhs.m_z;
    return *this;
  }

  Vec3<T>& operator=(const Vec4<T>& v) {
    m_x = v.x();
    m_y = v.y();
    m_z = v.z();
    return *this;
  }

  Vec3<T> operator+() const { return Vec3<T>(m_x, m_y, m_z); };
  Vec3<T> operator-() const { return Vec3<T>(-m_x, -m_y, -m_z); }

  Vec3<T>& operator++();
  Vec3<T>& operator--();
  Vec3<T> operator++(int);
  Vec3<T> operator--(int);

  Vec3<T>& operator+=(T num);
  Vec3<T>& operator+=(const Vec3<T>& v);
  Vec3<T>& operator-=(T num);
  Vec3<T>& operator-=(const Vec3<T>& v);
  Vec3<T>& operator*=(T num);

  Vec3<T>& normalize();
  float length() const { return sqrt(x() * x() + y() * y() + z() * z()); }

  bool isValid() {
    if (m_x * 0. != m_x * 0.) {
      return false;
    }
    if (m_y * 0. != m_y * 0.) {
      return false;
    }
    if (m_z * 0. != m_z * 0.) {
      return false;
    }

    return true;
  }

  void zero() {
    m_x = T{};
    m_y = T{};
    m_z = T{};
  }

 private:
  T m_x = T{};
  T m_y = T{};
  T m_z = T{};
};

typedef Vec3<double> Vec3d;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

//--------------------------------------------
// Overloaded Member operators (input, output)
//--------------------------------------------

template <typename T>
Vec3<T>& Vec3<T>::operator++() {
  *this += 1;  // use of += member operator
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator--() {
  *this -= 1;
  return *this;
}

template <typename T>
Vec3<T> Vec3<T>::operator++(int) {
  Vec3<T> temp(m_x, m_y, m_z);
  ++(*this);  // use of prefix ++ operator
  return temp;
}

template <typename T>
Vec3<T> Vec3<T>::operator--(int) {
  Vec3<T> temp(m_x, m_y, m_z);
  --(*this);
  return temp;
}

template <typename T>
Vec3<T>& Vec3<T>::operator+=(T num) {
  *this = (*this) + num;  // use of binary + operator
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& v) {
  *this = (*this) + v;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator-=(T num) {
  *this = (*this) - num;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& v) {
  *this = (*this) - v;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator*=(T num) {
  *this = (*this) * num;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::normalize() {
  T norm = this->length() + 1.E-30;
  *this = (*this) / norm;
  return *this;
}

//--------------------------------------------
// Overloaded I/O operators (input, output)
//--------------------------------------------

template <typename T>
inline std::istream& operator>>(std::istream& in, Vec3<T>& v) {
  T x, y, z;
  in >> x >> y >> z;
  v.setXYZ(x, y, z);
  return in;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vec3<T>& v) {
  out << "(" << v.x() << "," << v.y() << "," << v.z() << ")" << std::endl;
  return out;
}

//--------------------------------------------
// Overloaded Normal Function operators (input, output)
//--------------------------------------------

template <typename T>
Vec3<T> operator+(const Vec3<T>& v1, const Vec3<T>& v2) {
  return Vec3<T>(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

template <typename T>
Vec3<T> operator+(const Vec3<T>& v, T num) {
  return Vec3<T>(v.x() + num, v.y() + num, v.z() + num);
}

template <typename T>
Vec3<T> operator+(T num, const Vec3<T>& v) {
  return v + num;
}

template <typename T>
Vec3<T> operator-(const Vec3<T>& v1, const Vec3<T>& v2) {
  return Vec3<T>(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

template <typename T>
Vec3<T> operator-(const Vec3<T>& v, T num) {
  return Vec3<T>(v.x() - num, v.y() - num, v.z() - num);
}

template <typename T>
Vec3<T> operator-(T num, const Vec3<T>& v) {
  return v - num;
}

template <typename T>
Vec3<T> operator*(const Vec3<T>& v1, const Vec3<T>& v2) {
  return Vec3<T>(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

template <typename T>
Vec3<T> operator*(const Vec3<T>& v, T num) {
  return Vec3<T>(v.x() * num, v.y() * num, v.z() * num);
}

template <typename T>
Vec3<T> operator*(T num, const Vec3<T>& v) {
  return v * num;
}

template <typename T>
Vec3<T> operator/(const Vec3<T>& v1, const Vec3<T>& v2) {
  auto d = v2 + static_cast<T>(1.E-30);
  return Vec3<T>(v1.x() / d.x(), v1.y() / d.y(), v1.z() / d.z());
}

template <typename T>
Vec3<T> operator/(const Vec3<T>& v, T num) {
  num += 1.E-30;
  return Vec3<T>(v.x() / num, v.y() / num, v.z() / num);
}

template <typename T>
bool operator==(const Vec3<T>& v1, const Vec3<T>& v2) {
  return (v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z());
}

template <typename T>
bool operator!=(const Vec3<T>& v1, const Vec3<T>& v2) {
  return !(v1 == v2);
}

template <typename T>
T dot(const Vec3<T>& v1, const Vec3<T>& v2) {
  Vec3<T> v = v1 * v2;
  return v.x() + v.y() + v.z();
}

template <typename T>
Vec3<T> cross(const Vec3<T>& v1, const Vec3<T>& v2) {
  T x = v1.y() * v2.z() - v1.z() * v2.y();
  T y = v1.z() * v2.x() - v1.x() * v2.z();
  T z = v1.x() * v2.y() - v1.y() * v2.x();
  return Vec3<T>(x, y, z);
}

template <typename T>
Vec3<T> getUnitVectorOf(const Vec3<T>& v) {
  return v / static_cast<T>(v.length() + 1.E-30);
}

template <typename T>
Vec3<T> reflect(const Vec3<T>& in, const Vec3<T>& normal) {
  return in - normal * T{2} * dot(in, normal);
}

/*template <typename T>
Vec3<T> randomVector(
    const T& a = 0.,
    const T& b = 1.)  //!< Returns a vector with uniformly random
                      //!< components in the interval [a,b].
{
  std::uniform_real_distribution<T> rand(a, b);
  return Vec3<T>(rand(gen), rand(gen), rand(gen));
}

template <typename T>
Vec3<T>
randomVectorOnUnitSphere()  //!< Returns a random vector on a unit sphere.
{
  std::uniform_real_distribution<T> rand(0., 1.);
  T xi1, xi2, dsq = 2;
  while (dsq >= 1.) {
    xi1 = 1. - 2. * rand(gen);
    xi2 = 1. - 2. * rand(gen);
    dsq = xi1 * xi1 + xi2 * xi2;
  }

  T ranh = 2. * sqrt(1. - dsq);
  T dmx = xi1 * ranh;
  T dmy = xi2 * ranh;
  T dmz = 1. - 2. * dsq;

  Vec3<T> ret(dmx, dmy, dmz);
  return ret;
}*/
