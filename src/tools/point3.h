#pragma once

#include <cassert>
#include <sstream>

#include "vec3.h"
#include "vec4.h"

template <class T>
class Vec4;

template <class T>
class Vec3;

template <class T>
class Point3 {
 public:
  Point3<T>(const T &x = T(), const T &y = T(), const T &z = T())
      : m_x(x), m_y(y), m_z(z) {}

  Point3<T>(const Point3<T> &p) : m_x(p.x()), m_y(p.y()), m_z(p.z()) {}
  Point3<T>(const Vec4<T> &v) : m_x(v.x()), m_y(v.y()), m_z(v.z()) {}

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

  Point3<T> &operator=(const Vec4<T> &v) {
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

  Vec3<T> operator-(const Point3<T> &p) const {  // Point - Point = Vector
    return Vec3<T>(x() - p.x(), y() - p.y(), z() - p.z());
  }

  Vec3<T> operator+(const Point3<T> &p) const {
    return Vec3<T>(x() + p.x(), y() + p.y(), z() + p.z());
  }

  Point3<T> operator-(const Vec3<T> &v) const {  // Point - Vector = Point
    return Point3<T>(x() - v.x(), y() - v.y(), z() - v.z());
  }

  Point3<T> &operator-=(const Vec3<T> &v) {
    m_x -= v.x();
    m_y -= v.y();
    m_z -= v.z();
    return *this;
  }

  // auto operator<=>(const Point3<T> &) const = default;

 private:
  T m_x, m_y, m_z;
};

typedef Point3<double> Point3d;
typedef Point3<float> Point3f;
typedef Point3<int> Point3i;

template <typename T>
Vec3<T> operator-(const Vec3<T> &v,      // TODO: Cannot be (Smth is wrong)
                  const Point3<T> &p) {  // Vector - Point = Vector
  return Vec3<T>(v.x() - p.x(), v.y() - p.y(), v.z() - p.z());
}

template <typename T>
Vec3<T> operator+(const Vec3<T> &v, const Point3<T> &p) {
  return Vec3<T>(v.x() + p.x(), v.y() + p.y(), v.z() + p.z());
}

template <typename T>
Point3<T> operator+(const Point3<T> &p, const T &v) {
  return Point3<T>(p.x() + v, p.y() + v, p.z() + v);
}

template <typename T>
bool operator==(const Point3<T> &p1, const Point3<T> &p2) {
  return (p1.x() == p2.x()) && (p1.y() == p2.y()) && (p1.z() == p2.z());
}

template <typename T>
std::stringstream &operator<<(std::stringstream &out, const Point3<T> &p) {
  out << "(" << p.x() << "," << p.y() << "," << p.z() << ")";
  return out;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const Point3<T> &p) {
  out << "(" << p.x() << "," << p.y() << "," << p.z() << ")";
  return out;
}

template <typename T>
Point3<T> operator*(const Point3<T> &v, const T &num) {
  return Point3<T>(v.x() * num, v.y() * num, v.z() * num);
}

template <typename T>
Point3<T> operator*(const T &num, const Point3<T> &v) {
  return v * num;
}
