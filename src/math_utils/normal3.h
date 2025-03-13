#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

template <class T>
class Vec4;

template <class T>
class Vec3;

template <class T>
class Point3;

template <class T>
class Normal3 {
 public:
  Normal3() = default;
  Normal3(T p1, T p2, T p3) : m_x{p1}, m_y{p2}, m_z{p3} {}
  explicit Normal3(const Vec4<T>& v) : m_x{v.x()}, m_y{v.y()}, m_z{v.z()} {}
  explicit Normal3(const Point3<T>& p) : m_x{p.x()}, m_y{p.y()}, m_z{p.z()} {}
  explicit Normal3(const Vec3<T>& v) : m_x{v.x()}, m_y{v.y()}, m_z{v.z()} {}

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

  Normal3<T>& operator=(const Vec4<T>& v) {
    m_x = v.x();
    m_y = v.y();
    m_z = v.z();
    return *this;
  }

  auto operator<=>(const Normal3<T>&) const = default;

  Normal3<T> operator+() const { return Normal3<T>(m_x, m_y, m_z); };
  Normal3<T> operator-() const { return Normal3<T>(-m_x, -m_y, -m_z); }

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

using Normal3D = Normal3<float>;

//--------------------------------------------
// Overloaded Member operators (input, output)
//--------------------------------------------

template <typename T>
void Normal3<T>::normalize() {
  *this = (*this) / (this->length() + 1.E-30f);
}

//--------------------------------------------
// Overloaded I/O operators (input, output)
//--------------------------------------------

template <typename T>
inline std::istream& operator>>(std::istream& in, Normal3<T>& n) {
  T x, y, z;
  in >> x >> y >> z;
  n.set(x, y, z);
  return in;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Normal3<T>& n) {
  out << "(" << n.x() << "," << n.y() << "," << n.z() << ")" << std::endl;
  return out;
}

//--------------------------------------------
// Overloaded Normal Function operators (input, output)
//--------------------------------------------
template <typename T>
bool operator==(const Normal3<T>& n, const Vec3<T>& v) {
  return (n.x() == v.x() && n.y() == v.y() && n.z() == v.z());
}

template <typename T>
bool operator!=(const Normal3<T>& n, const Vec3<T>& v) {
  return !(n == v);
}

template <typename T>
Normal3<T> operator+(const Normal3<T>& n1, const Normal3<T>& n2) {
  return Normal3<T>(n1.x() + n2.x(), n1.y() + n2.y(), n1.z() + n2.z());
}

template <typename T>
Normal3<T> operator+(const Normal3<T>& n, const Vec3<T>& v) {
  return Vec3<T>(n.x() + v.x(), n.y() + v.y(), n.z() + v.z());
}

template <typename T>
Normal3<T> operator+(const Vec3<T>& v, const Normal3<T>& n) {
  return n + v;
}

template <typename T>
Normal3<T> operator+(const Normal3<T>& n, T num) {
  return Normal3<T>(n.x() + num, n.y() + num, n.z() + num);
}

template <typename T>
Normal3<T> operator+(T num, const Normal3<T>& v) {
  return v + num;
}

template <typename T>
Normal3<T> operator-(const Normal3<T>& n1, const Normal3<T>& n2) {
  return Normal3<T>(n1.x() - n2.x(), n1.y() - n2.y(), n1.z() - n2.z());
}

template <typename T>
Normal3<T> operator-(const Normal3<T>& n, T num) {
  return Normal3<T>(n.x() - num, n.y() - num, n.z() - num);
}

template <typename T>
Normal3<T> operator-(T num, const Normal3<T>& n) {
  return n - num;
}

template <typename T>
Normal3<T> operator*(const Normal3<T>& n1, const Normal3<T>& n2) {
  return Normal3<T>(n1.x() * n2.x(), n1.y() * n2.y(), n1.z() * n2.z());
}

template <typename T>
Normal3<T> operator*(const Normal3<T>& n, const Vec3<T>& v) {
  return Normal3<T>(n.x() * v.x(), n.y() * v.y(), n.z() * v.z());
}

template <typename T>
Normal3<T> operator*(const Vec3<T>& v, const Normal3<T>& n) {
  return n * v;
}

template <typename T>
Normal3<T> operator*(const Normal3<T>& n, T num) {
  return Normal3<T>(n.x() * num, n.y() * num, n.z() * num);
}

template <typename T>
Normal3<T> operator*(T num, const Normal3<T>& n) {
  return n * num;
}

template <typename T>
Normal3<T> operator/(const Normal3<T>& n1, const Normal3<T>& n2) {
  auto d = n2 + static_cast<T>(1.E-30);
  return Normal3<T>(n1.x() / d.x(), n1.y() / d.y(), n1.z() / d.z());
}

template <typename T>
Normal3<T> operator/(const Normal3<T>& n, T num) {
  num += 1.E-30;
  return Normal3<T>(n.x() / num, n.y() / num, n.z() / num);
}

template <typename T>
T dot(const Normal3<T>& n1, const Normal3<T>& n2) {
  Normal3<T> n = n1 * n2;
  return n.x() + n.y() + n.z();
}

template <typename T>
T dot(const Normal3<T>& n, const Vec3<T>& v) {
  Normal3<T> u = n * v;
  return u.x() + u.y() + u.z();
}

template <typename T>
T dot(const Vec3<T>& v, const Normal3<T>& n) {
  return dot(n, v);
}

template <typename T>
Normal3<T> getUnitVectorOf(const Normal3<T>& n) {
  return n / static_cast<T>(n.length() + 1.E-30);
}
