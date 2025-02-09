#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

template <class T>
class Vec2 {
 public:
  Vec2() = default;
  Vec2(T p1, T p2) : m_x{p1}, m_y{p2} {}

  T x() const { return m_x; }
  T y() const { return m_y; }

  void setX(T num) { m_x = num; }
  void setY(T num) { m_y = num; }
  void set(T num) { m_x = m_y = num; }
  void set(T num1, T num2) {
    m_x = num1;
    m_y = num2;
  }

  T operator[](int i) const {
    assert(i >= 0 && i <= 1);
    if (i == 0) return m_x;
    return m_y;
  }

  T& operator[](int i) {
    assert(i >= 0 && i <= 1);
    if (i == 0) return m_x;
    return m_y;
  }

  auto operator<=>(const Vec2<T>&) const = default;

  Vec2<T> operator+() const { return Vec2<T>(m_x, m_y); };
  Vec2<T> operator-() const { return Vec2<T>(-m_x, -m_y); }

  void normalize();
  float length() const { return sqrt(m_x * m_x + m_y * m_y); }

 private:
  T m_x = T{};
  T m_y = T{};
};

using Vec2D = Vec2<float>;

//--------------------------------------------
// Overloaded Member operators
//--------------------------------------------

template <typename T>
void Vec2<T>::normalize() {
  *this = (*this) / (this->length() + 1.E-30);
}

//--------------------------------------------
// Overloaded I/O operators (input, output)
//--------------------------------------------

template <typename T>
inline std::istream& operator>>(std::istream& in, Vec2<T>& v) {
  T x, y;
  in >> x >> y;
  v.set(x, y);
  return in;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vec2<T>& v) {
  out << "(" << v.x() << "," << v.y() << ")";
  return out;
}

//--------------------------------------------
// Overloaded operators as normal functions
// Binary operator (+, -, *)
//--------------------------------------------

template <typename T>
Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2) {
  return Vec2<T>(v1.x() + v2.x(), v1.y() + v2.y());
}

template <typename T>
Vec2<T> operator+(const Vec2<T>& v, T num) {
  return Vec2<T>(v.x() + num, v.y() + num);
}

template <typename T>
Vec2<T> operator+(T num, const Vec2<T>& v) {
  return v + num;
}

template <typename T>
Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2) {
  return Vec2<T>(v1.x() - v2.x(), v1.y() - v2.y());
}

template <typename T>
Vec2<T> operator-(const Vec2<T>& v, T num) {
  return Vec2<T>(v.x() - num, v.y() - num);
}

template <typename T>
Vec2<T> operator-(T num, const Vec2<T>& v) {
  return v - num;
}

template <typename T>
Vec2<T> operator*(const Vec2<T>& v1, const Vec2<T>& v2) {
  return Vec2<T>(v1.x() * v2.x(), v1.y() * v2.y());
}

template <typename T>
Vec2<T> operator*(const Vec2<T>& v, T num) {
  return Vec2<T>(v.x() * num, v.y() * num);
}

template <typename T>
Vec2<T> operator*(T num, const Vec2<T>& v) {
  return v * num;
}

template <typename T>
Vec2<T> operator/(const Vec2<T>& v1, const Vec2<T>& v2) {
  auto eps = 1.E-30;
  auto d = v2 + eps;
  return Vec2<T>(v1.x() / d.x(), v1.y() / d.y());
}

template <typename T>
Vec2<T> operator/(const Vec2<T>& v, T num) {
  num += 1.E-30;
  return Vec2<T>(v.x() / num, v.y() / num);
}

//--------------------------------------------
// Other vector operations
//--------------------------------------------

template <typename T>
T dot(const Vec2<T>& v1, const Vec2<T>& v2) {
  auto v = v1 * v2;
  return v.x() + v.y();
}

template <typename T>
Vec2<T> getUnitVectorOf(const Vec2<T>& v) {
  return v / (v.length() + 1.E-30);
}
