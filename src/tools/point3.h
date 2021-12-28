#pragma once

#include <cassert>

#include "vec3.h"

template <class T>
class Point3 {
 public:
  Point3<T>(const T &x = T(), const T &y = T(), const T &z = T())
      : m_x(x), m_y(y), m_z(z) {}

  T x() const { return m_x; }
  T y() const { return m_y; }
  T z() const { return m_z; }

  void setX(const T &x) { m_x = x; }
  void setY(const T &y) { m_y = y; }
  void setZ(const T &z) { m_z = z; }
  void setAll(const T &n) { m_x = m_y = m_z = n; }

  T operator[](int i) const {
    assert(i >= 0 && i <= 2);
    if (i == 0) return m_x;
    if (i == 1) return m_y;
    return m_z;
  }

  T &operator[](int i) {
    assert(i >= 0 && i <= 2);
    if (i == 0) return m_x;
    if (i == 1) return m_y;
    return m_z;
  }

  Point3<T> &operator=(const Point3<T> &v) {
    m_x = v.x();
    m_y = v.y();
    m_z = v.z();
    return *this;
  }

  Point3<T> operator+(const Vec3<T> &v) const {
    return Point3<T>(x() + v.x(), y() + v.y(), z() + v.z());
  }

  Point3<T> &operator+=(const Vec3<T> &v) {
    m_x += v.x();
    m_y += v.y();
    m_z += v.z();
    return *this;
  }

  Vec3<T> operator-(const Point3<T> &p) const {
    return Vec3<T>(x() - p.x(), y() - p.y(), z() - p.z());
  }

  Point3<T> operator-(const Vec3<T> &v) const {
    return Point3<T>(x() - v.x(), y() - v.y(), z() - v.z());
  }

  Point3<T> &operator-=(const Vec3<T> &v) {
    m_x -= v.x();
    m_y -= v.y();
    m_z -= v.z();
    return *this;
  }

 private:
  T m_x, m_y, m_z;
};

typedef Point3<double> Point3d;
typedef Point3<float> Point3f;
typedef Point3<int> Point3i;
