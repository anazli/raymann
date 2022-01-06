#ifndef VEC2_H
#define VEC2_H

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

template <class T>
class Vec2 {
 public:
  Vec2(const T& p1 = 0., const T& p2 = 0.) : m_x{p1}, m_y{p2} {}
  Vec2(const Vec2<T>& v) : m_x{v.x()}, m_y{v.y()} {}

  T x() const { return m_x; }
  T y() const { return m_y; }

  void setX(const T& p) { m_x = p; }
  void setY(const T& p) { m_y = p; }

  void setXY(const T& num) {
    setX(num);
    setY(num);
  }

  void setXY(const T& p1, const T& p2) {
    setX(p1);
    setY(p2);
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

  Vec2& operator=(const Vec2& v) {
    m_x = v.x();
    m_y = v.y();
    return *this;
  }

  Vec2<T> operator+() const { return Vec2<T>(m_x, m_y); };
  Vec2<T> operator-() const { return Vec2<T>(-m_x, -m_y); }
  Vec2<T>& operator++();
  Vec2<T>& operator--();
  Vec2<T> operator++(int);
  Vec2<T> operator--(int);

  Vec2<T>& operator+=(const T& num);

  Vec2<T>& operator+=(const Vec2<T>& v);
  Vec2<T>& operator-=(const T& num);

  Vec2<T>& operator-=(const Vec2<T>& v);
  Vec2<T>& operator*=(const T& num);

  Vec2<T>& normalize();
  double length() const { return sqrt(x() * x() + y() * y()); }

  bool isValid() {
    if (m_x * 0. != m_x * 0.) {
      return false;
    }
    if (m_y * 0. != m_y * 0.) {
      return false;
    }

    return true;
  }

 private:
  T m_x;
  T m_y;
};

typedef Vec2<double> Vec2d;
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

//--------------------------------------------
// Overloaded Member operators
//--------------------------------------------

template <typename T>
Vec2<T>& Vec2<T>::operator++() {
  *this += 1;  // use of += member operator
  return *this;
}

template <typename T>
Vec2<T>& Vec2<T>::operator--() {
  *this -= 1;
  return *this;
}

template <typename T>
Vec2<T> Vec2<T>::operator++(int) {
  Vec2 temp(m_x, m_y);
  ++(*this);  // use of prefix ++ operator
  return temp;
}

template <typename T>
Vec2<T> Vec2<T>::operator--(int) {
  Vec2 temp(m_x, m_y);
  --(*this);
  return temp;
}

template <typename T>
Vec2<T>& Vec2<T>::operator+=(const T& num) {
  *this = (*this) + num;  // use of binary + operator
  return *this;
}

template <typename T>
Vec2<T>& Vec2<T>::operator+=(const Vec2& v) {
  *this = (*this) + v;
  return *this;
}

template <typename T>
Vec2<T>& Vec2<T>::operator-=(const T& num) {
  *this = (*this) - num;
  return *this;
}

template <typename T>
Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& v) {
  *this = (*this) - v;
  return *this;
}

template <typename T>
Vec2<T>& Vec2<T>::operator*=(const T& num) {
  *this = (*this) * num;
  return *this;
}

template <typename T>
Vec2<T>& Vec2<T>::normalize() {
  double norm = this->length();
  *this = (*this) / (norm + 1.E-30);
  return *this;
}

//--------------------------------------------
// Overloaded I/O operators (input, output)
//--------------------------------------------

template <typename T>
inline std::istream& operator>>(std::istream& in, Vec2<T>& v) {
  T x, y;
  in >> x >> y;
  v.setXY(x, y);
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
Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2)

{
  return Vec2<T>(v1.x() + v2.x(), v1.y() + v2.y());
}

template <typename T>
Vec2<T> operator+(const Vec2<T>& v, const T& num)

{
  return Vec2<T>(v.x() + num, v.y() + num);
}

template <typename T>
Vec2<T> operator+(const T& num, const Vec2<T>& v) {
  return v + num;
}

template <typename T>
Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2) {
  return Vec2<T>(v1.x() - v2.x(), v1.y() - v2.y());
}

template <typename T>
Vec2<T> operator-(const Vec2<T>& v, const T& num) {
  return Vec2<T>(v.x() - num, v.y() - num);
}

template <typename T>
Vec2<T> operator-(const T& num, const Vec2<T>& v) {
  return v - num;
}

template <typename T>
Vec2<T> operator*(const Vec2<T>& v1, const Vec2<T>& v2) {
  return Vec2<T>(v1.x() * v2.x(), v1.y() * v2.y());
}

template <typename T>
Vec2<T> operator*(const Vec2<T>& v, const T& num) {
  return Vec2<T>(v.x() * num, v.y() * num);
}

template <typename T>
Vec2<T> operator*(const T& num, const Vec2<T>& v) {
  return v * num;
}

template <typename T>
Vec2<T> operator/(const Vec2<T>& v1, const Vec2<T>& v2) {
  if (v2.x() == (T)0 || v2.y() == (T)0) throw "Division by zero!";
  return Vec2<T>(v1.x() / v2.x(), v1.y() / v2.y());
}

template <typename T>
Vec2<T> operator/(const Vec2<T>& v, const T& num) {
  T e = 1.E-30;
  return Vec2<T>(v.x() / (num + e), v.y() / (num + e));
}

template <typename T>
bool operator==(const Vec2<T>& v1, const Vec2<T>& v2) {
  return (v1.x() == v2.x() && v1.y() == v2.y());
}

template <typename T>
bool operator!=(const Vec2<T>& v1, const Vec2<T>& v2) {
  return !(v1 == v2);
}

//--------------------------------------------
// General methods
//--------------------------------------------

template <typename T>
T dot(const Vec2<T>& v1, const Vec2<T>& v2) {
  Vec2<T> v = v1 * v2;
  return v.x() + v.y();
}

template <typename T>
Vec2<T> getUnitVectorOf(const Vec2<T>& v) {
  return v / (v.length() + (T)1.E-30);
}

#endif  // VEC2_H
