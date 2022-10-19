#ifndef VEC3_H
#define VEC3_H

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

#include "vec4.h"

template <class T>
class Vec3 {
 public:
  Vec3<T>(const T& p1 = static_cast<T>(0), const T& p2 = static_cast<T>(0),
          const T& p3 = static_cast<T>(0))
      : m_x{p1}, m_y{p2}, m_z{p3} {}

  Vec3<T>(const Vec3<T>& v) : m_x{v.x()}, m_y{v.y()}, m_z{v.z()} {}
  Vec3<T>(const Vec4<T>& v) : m_x{v.x()}, m_y{v.y()}, m_z{v.z()} {}

  T x() const { return m_x; }
  T y() const { return m_y; }
  T z() const { return m_z; }

  void setX(const T& p) { m_x = p; }
  void setY(const T& p) { m_y = p; }
  void setZ(const T& p) { m_z = p; }

  void setXYZ(const T& num) {
    setX(num);
    setY(num);
    setZ(num);
  }

  void setXYZ(const T& p1, const T& p2, const T& p3) {
    setX(p1);
    setY(p2);
    setZ(p3);
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

  Vec3<T>& operator=(const Vec3<T>& v) {
    m_x = v.x();
    m_y = v.y();
    m_z = v.z();
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

  Vec3<T>& operator+=(const T& num);
  Vec3<T>& operator+=(const Vec3<T>& v);
  Vec3<T>& operator-=(const T& num);
  Vec3<T>& operator-=(const Vec3<T>& v);
  Vec3<T>& operator*=(const T& num);

  Vec3<T>& normalize();
  double length() const { return sqrt(x() * x() + y() * y() + z() * z()); }

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
    m_x = (T)0;
    m_y = (T)0;
    m_z = (T)0;
  }

 private:
  T m_x;
  T m_y;
  T m_z;
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
Vec3<T>& Vec3<T>::operator+=(const T& num) {
  *this = (*this) + num;  // use of binary + operator
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& v) {
  *this = (*this) + v;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator-=(const T& num) {
  *this = (*this) - num;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& v) {
  *this = (*this) - v;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator*=(const T& num) {
  *this = (*this) * num;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::normalize() {
  T norm = this->length() + (T)1.E-30;
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
Vec3<T> operator+(const Vec3<T>& v, const T& num) {
  return Vec3<T>(v.x() + num, v.y() + num, v.z() + num);
}

template <typename T>
Vec3<T> operator+(const T& num, const Vec3<T>& v) {
  return v + num;
}

template <typename T>
Vec3<T> operator-(const Vec3<T>& v1, const Vec3<T>& v2) {
  return Vec3<T>(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

template <typename T>
Vec3<T> operator-(const Vec3<T>& v, const T& num) {
  return Vec3<T>(v.x() - num, v.y() - num, v.z() - num);
}

template <typename T>
Vec3<T> operator-(const T& num, const Vec3<T>& v) {
  return v - num;
}

template <typename T>
Vec3<T> operator*(const Vec3<T>& v1, const Vec3<T>& v2) {
  return Vec3<T>(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

template <typename T>
Vec3<T> operator*(const Vec3<T>& v, const T& num) {
  return Vec3<T>(v.x() * num, v.y() * num, v.z() * num);
}

template <typename T>
Vec3<T> operator*(const T& num, const Vec3<T>& v) {
  return v * num;
}

template <typename T>
Vec3<T> operator/(const Vec3<T>& v1, const Vec3<T>& v2) {
  if (v2.x() == 0. || v2.y() == 0. || v2.z() == 0.) throw "Division by zero!";
  return Vec3<T>(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}

template <typename T>
Vec3<T> operator/(const Vec3<T>& v, const T& num) {
  T e = 1.E-30;
  return Vec3<T>(v.x() / (num + e), v.y() / (num + e), v.z() / (num + e));
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
  return v / (v.length() + 1.E-30);
}

template <typename T>
Vec3<T> reflect(const Vec3<T>& in, const Vec3<T>& normal) {
  return in - normal * (T)2 * dot(in, normal);
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

#endif  // VEC3_H
