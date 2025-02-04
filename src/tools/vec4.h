#pragma once

#include <cassert>
#include <iostream>
#include <random>

#include "point3.h"
#include "vec3.h"

template <typename T>
class Vec3;

template <typename T>
class Point3;

template <class T>
class Vec4 {
 public:
  Vec4() = default;
  Vec4(T p1, T p2, T p3, T p4) : m_x{p1}, m_y{p2}, m_z{p3}, m_w{p4} {}

  Vec4(const Vec4<T>& v) : m_x{v.m_x}, m_y{v.m_y}, m_z{v.m_z}, m_w{v.m_w} {}
  Vec4(const Vec3<T>& v) : m_x{v.x()}, m_y{v.y()}, m_z(v.z()), m_w{0} {}
  Vec4(const Point3<T>& p) : m_x{p.x()}, m_y{p.y()}, m_z{p.z()}, m_w{1} {}

  T x() const { return m_x; }
  T y() const { return m_y; }
  T z() const { return m_z; }
  T w() const { return m_w; }

  void setX(T num) { m_x = num; }
  void setY(T num) { m_y = num; }
  void setZ(T num) { m_z = num; }
  void setW(T num) { m_w = num; }

  void setXYZW(T num) { m_x = m_y = m_z = m_w = num; }

  void setXYZW(T num1, T num2, T num3, T num4) {
    m_x = num1;
    m_y = num2;
    m_z = num3;
    m_w = num4;
  }

  T operator[](int i) const {
    assert(i >= 0 && i <= 3);
    if (i == 0) return m_x;
    if (i == 1) return m_y;
    if (i == 2) return m_z;
    return m_w;
  }

  T& operator[](int i) {
    assert(i >= 0 && i <= 3);
    if (i == 0) return m_x;
    if (i == 1) return m_y;
    if (i == 2) return m_z;
    return m_w;
  }

  Vec4<T>& operator=(const Vec4<T>& v) {
    m_x = v.m_x;
    m_y = v.m_y;
    m_z = v.m_z;
    m_w = v.m_w;
    return *this;
  }

  Vec4<T>& operator=(const Vec3<T>& v) {
    m_x = v.x();
    m_y = v.y();
    m_z = v.z();
    m_w = T{0};
    return *this;
  }

  Vec4<T>& operator=(const Point3<T>& p) {
    m_x = p.x();
    m_y = p.y();
    m_z = p.z();
    m_w = T{1};
    return *this;
  }

  Vec4<T> operator+() const { return Vec4<T>(m_x, m_y, m_z, m_w); };
  Vec4<T> operator-() const { return Vec4<T>(-m_x, -m_y, -m_z, -m_w); }

  Vec4<T>& operator+=(T num);

  Vec4<T>& operator+=(const Vec4<T>& v);
  Vec4<T>& operator-=(T num);

  Vec4<T>& operator-=(const Vec4<T>& v);
  Vec4<T>& operator*=(T num);

  Vec4<T>& normalize();
  double length() const {
    return sqrt(x() * x() + y() * y() + z() * z() + w() * w());
  }

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
    if (m_w * 0. != m_w * 0.) {
      return false;
    }

    return true;
  }

  void zero() {
    m_x = T{};
    m_y = T{};
    m_z = T{};
    m_w = T{};
  }

 private:
  T m_x = T{};
  T m_y = T{};
  T m_z = T{};
  T m_w = T{};
};

typedef Vec4<double> Vec4d;
typedef Vec4<float> Vec4f;
typedef Vec4<int> Vec4i;

//--------------------------------------------
// Overloaded Member operators
//--------------------------------------------

template <typename T>
Vec4<T>& Vec4<T>::operator+=(T num) {
  *this = (*this) + num;  // use of binary + operator
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& v) {
  *this = (*this) + v;
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator-=(T num) {
  *this = (*this) - num;
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& v) {
  *this = (*this) - v;
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator*=(T num) {
  *this = (*this) * num;
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::normalize() {
  T norm = this->length();
  *this = (*this) / (norm + (T)1.E-30);
  return *this;
}

//--------------------------------------------
// Overloaded I/O operators (input, output)
//--------------------------------------------

template <typename T>
std::istream& operator>>(std::istream& in, Vec4<T>& v) {
  T x, y, z, w;
  in >> x >> y >> z >> w;
  v.setXYZW(x, y, z, w);
  return in;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Vec4<T>& v) {
  out << "(" << v.x() << "," << v.y() << "," << v.z() << "," << v.w() << ")";
  return out;
}

template <typename T>
Vec4<T> operator+(const Vec4<T>& v1, const Vec4<T>& v2) {
  return Vec4<T>(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z(),
                 v1.w() + v2.w());
}

template <typename T>
Vec4<T> operator+(const Vec4<T>& v, T num) {
  return Vec4<T>(v.x() + num, v.y() + num, v.z() + num, v.w() + num);
}

template <typename T>
Vec4<T> operator+(T num, const Vec4<T>& v) {
  return v + num;
}

template <typename T>
Vec4<T> operator-(const Vec4<T>& v1, const Vec4<T>& v2) {
  return Vec4<T>(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z(),
                 v1.w() - v2.w());
}

template <typename T>
Vec4<T> operator-(const Vec4<T>& v, T num) {
  return Vec4<T>(v.x() - num, v.y() - num, v.z() - num, v.w() - num);
}

template <typename T>
Vec4<T> operator-(T num, const Vec4<T>& v) {
  return v - num;
}

template <typename T>
Vec4<T> operator*(const Vec4<T>& v1, const Vec4<T>& v2) {
  return Vec4<T>(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z(),
                 v1.w() * v2.w());
}

template <typename T>
Vec4<T> operator*(const Vec4<T>& v, T num) {
  return Vec4<T>(v.x() * num, v.y() * num, v.z() * num, v.w() * num);
}

template <typename T>
Vec4<T> operator*(T num, const Vec4<T>& v) {
  return v * num;
}

template <typename T>
Vec4<T> operator/(const Vec4<T>& v1, const Vec4<T>& v2) {
  if (v2.x() == 0. || v2.y() == 0. || v2.z() == 0. || v2.w() == 0.)
    throw "Division by zero!";
  return Vec4<T>(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z(),
                 v1.w() / v2.w());
}

template <typename T>
Vec4<T> operator/(const Vec4<T>& v, T num) {
  double e = 1.E-30;
  return Vec4<T>(v.x() / (num + e), v.y() / (num + e), v.z() / (num + e),
                 v.w() / (num + e));
}

template <typename T>
bool operator==(const Vec4<T>& v1, const Vec4<T>& v2) {
  return (v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z() &&
          v1.w() == v2.w());
}

template <typename T>
bool operator!=(const Vec4<T>& v1, const Vec4<T>& v2) {
  return !(v1 == v2);
}

template <typename T>
double dot(const Vec4<T>& v1, const Vec4<T>& v2) {
  Vec4<T> v = v1 * v2;
  return v.x() + v.y() + v.z() + v.w();
}

template <typename T>
Vec4<T> getUnitVectorOf(const Vec4<T>& v) {
  return v / (v.length() + 1.E-30);
}
