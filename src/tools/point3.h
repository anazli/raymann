#pragma once

#include <cassert>
#include <sstream>

template <class T>
class Vec4;

template <class T>
class Vec3;

template <class T>
class Normal3;

template <class T>
class Point3 {
 public:
  Point3() = default;
  Point3(T x, T y, T z) : m_x(x), m_y(y), m_z(z) {}
  explicit Point3(const Vec4<T> &v) : m_x(v.x()), m_y(v.y()), m_z(v.z()) {}
  explicit Point3(const Vec3<T> &v) : m_x(v.x()), m_y(v.y()), m_z(v.z()) {}
  explicit Point3(const Normal3<T> &n) : m_x(n.x()), m_y(n.y()), m_z(n.z()) {}

  T x() const { return m_x; }
  T y() const { return m_y; }
  T z() const { return m_z; }

  void setX(T x) { m_x = x; }
  void setY(T y) { m_y = y; }
  void setZ(T z) { m_z = z; }
  void setAll(T n) { m_x = m_y = m_z = n; }

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

  Point3<T> &operator=(const Vec4<T> &vec4) {
    m_x = vec4.x();
    m_y = vec4.y();
    m_z = vec4.z();
    return *this;
  }

  auto operator<=>(const Point3<T> &) const = default;

  Point3<T> operator+(const Vec3<T> &vec3) const {
    return Point3<T>(m_x + vec3.x(), m_y + vec3.y(), m_z + vec3.z());
  }

  Vec3<T> operator+(const Point3<T> &rhs) const {
    return Vec3<T>(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
  }

  Point3<T> operator-(const Vec3<T> &v) const {  // Point - Vector = Point
    return Point3<T>(m_x - v.x(), m_y - v.y(), m_z - v.z());
  }

  Vec3<T> operator-(const Point3<T> &rhs) const {  // Point - Point = Vector
    return Vec3<T>(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
  }

 private:
  T m_x = T{};
  T m_y = T{};
  T m_z = T{};
};

using Point3D = Point3<float>;

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
Point3<T> operator+(const Point3<T> &p, T num) {
  return Point3<T>(p.x() + num, p.y() + num, p.z() + num);
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
Point3<T> operator*(const Point3<T> &p, T num) {
  return Point3<T>(p.x() * num, p.y() * num, p.z() * num);
}

template <typename T>
Point3<T> operator*(T num, const Point3<T> &p) {
  return p * num;
}
