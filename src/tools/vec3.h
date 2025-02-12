#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

template <class T>
class Vec4;

template <class T>
class Point3;

template <class T>
class Normal3;

template <class T>
class Vec3 {
 public:
  Vec3() = default;
  Vec3(T p1, T p2, T p3) : m_x{p1}, m_y{p2}, m_z{p3} {}
  explicit Vec3(const Vec4<T>& v) : m_x{v.x()}, m_y{v.y()}, m_z{v.z()} {}
  explicit Vec3(const Point3<T>& v) : m_x{v.x()}, m_y{v.y()}, m_z{v.z()} {}
  explicit Vec3(const Normal3<T>& n) : m_x{n.x()}, m_y{n.y()}, m_z{n.z()} {}

  T x() const { return m_x; }
  T y() const { return m_y; }
  T z() const { return m_z; }

  void setX(T num) { m_x = num; }
  void setY(T num) { m_y = num; }
  void setZ(T num) { m_z = num; }
  void set(T num) { m_x = m_y = m_z = num; }
  void set(T num1, T num2, T num3) {
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

  Vec3<T>& operator=(const Vec4<T>& v) {
    m_x = v.x();
    m_y = v.y();
    m_z = v.z();
    return *this;
  }

  auto operator<=>(const Vec3<T>&) const = default;

  Vec3<T> operator+() const { return Vec3<T>(m_x, m_y, m_z); };
  Vec3<T> operator-() const { return Vec3<T>(-m_x, -m_y, -m_z); }

  void normalize();
  float length() const { return sqrt(x() * x() + y() * y() + z() * z()); }

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

using Vec3D = Vec3<float>;

//--------------------------------------------
// Overloaded Member operators (input, output)
//--------------------------------------------

template <typename T>
void Vec3<T>::normalize() {
  *this = (*this) / (this->length() + 1.E-30f);
}

//--------------------------------------------
// Overloaded I/O operators (input, output)
//--------------------------------------------

template <typename T>
inline std::istream& operator>>(std::istream& in, Vec3<T>& v) {
  T x, y, z;
  in >> x >> y >> z;
  v.set(x, y, z);
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
