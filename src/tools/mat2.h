#ifndef MAT2_H
#define MAT2_H

#include "vec2.h"

template <class T>
class Mat2 {
 public:
  Mat2<T>() {}
  Mat2<T>(const T& num) {
    m_vec[0].setXY(num);
    m_vec[1].setXY(num);
  }
  Mat2<T>(const Vec2<T>& row1, const Vec2<T>& row2) {
    m_vec[0] = row1;
    m_vec[1] = row2;
  }

  Mat2<T>& operator=(const Mat2<T>& c) {
    m_vec[0] = c[0];
    m_vec[1] = c[1];
    return *this;
  }

  Mat2<T>& operator+=(const Mat2<T>& c) {
    m_vec[0] += c[0];
    m_vec[1] += c[1];
    return *this;
  }

  Mat2<T>& operator+=(const T& num) {
    m_vec[0] += num;
    m_vec[1] += num;
    return *this;
  }

  Mat2<T>& operator-=(const Mat2<T>& c) {
    m_vec[0] -= c[0];
    m_vec[1] -= c[1];
    return *this;
  }

  Mat2<T>& operator-=(const T& num) {
    m_vec[0] -= num;
    m_vec[1] -= num;
    return *this;
  }

  Mat2<T>& operator*=(const T& num) {
    m_vec[0] *= num;
    m_vec[1] *= num;
    return *this;
  }

  Vec2<T> operator[](int i) const {
    assert(i >= 0 && i <= 1);
    if (i == 0) return m_vec[0];
    return m_vec[1];
  }

  Vec2<T>& operator[](int i) {
    assert(i >= 0 && i <= 1);
    if (i == 0) return m_vec[0];
    return m_vec[1];
  }

  double determinant() const {
    return m_vec[0].x() * m_vec[1].y() - m_vec[0].y() * m_vec[1].x();
  }

 private:
  Vec2<T> m_vec[2];
};

typedef Mat2<double> Mat2d;
typedef Mat2<float> Mat2f;
typedef Mat2<int> Mat2i;

template <typename T>
Mat2<T> operator+(const Mat2<T>& m1, const Mat2<T>& m2) {
  return Mat2<T>(m1[0] + m2[0], m1[1] + m2[1]);
}

template <typename T>
Mat2<T> operator-(const Mat2<T>& m1, const Mat2<T>& m2) {
  return Mat2<T>(m1[0] - m2[0], m1[1] - m2[1]);
}

template <typename T>
Mat2<T> operator*(const Mat2<T>& m1, const Mat2<T>& m2) {
  Mat2<T> ret;
  ret[0][0] = m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0];
  ret[0][1] = m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1];
  ret[1][0] = m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0];
  ret[1][1] = m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1];

  return ret;
}

#endif  // MAT2
