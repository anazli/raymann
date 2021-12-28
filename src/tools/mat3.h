#ifndef MAT3_H
#define MAT3_H

#include "mat2.h"
#include "vec3.h"

template <class T>
class Mat3 {
 public:
  Mat3<T>() {}
  Mat3<T>(const T& num) {
    m_vec[0].setXYZ(num);
    m_vec[1].setXYZ(num);
    m_vec[2].setXYZ(num);
  }
  Mat3<T>(const Vec3<T>& row1, const Vec3<T>& row2, const Vec3<T>& row3) {
    m_vec[0] = row1;
    m_vec[1] = row2;
    m_vec[2] = row3;
  }

  Mat3<T>& operator=(const Mat3<T>& c) {
    m_vec[0] = c[0];
    m_vec[1] = c[1];
    m_vec[2] = c[2];
    return *this;
  }

  Mat3<T>& operator+=(const Mat3<T>& c) {
    m_vec[0] += c[0];
    m_vec[1] += c[1];
    m_vec[2] += c[2];
    return *this;
  }

  Mat3<T>& operator+=(const T& num) {
    m_vec[0] += num;
    m_vec[1] += num;
    m_vec[2] += num;
    return *this;
  }

  Mat3<T>& operator-=(const Mat3<T>& c) {
    m_vec[0] -= c[0];
    m_vec[1] -= c[1];
    m_vec[2] -= c[2];
    return *this;
  }

  Mat3<T>& operator-=(const T& num) {
    m_vec[0] -= num;
    m_vec[1] -= num;
    m_vec[2] -= num;
    return *this;
  }

  Mat3<T>& operator*=(const T& num) {
    m_vec[0] *= num;
    m_vec[1] *= num;
    m_vec[2] *= num;
    return *this;
  }

  Vec3<T> operator[](int i) const {
    assert(i >= 0 && i <= 2);
    if (i == 0) return m_vec[0];
    if (i == 1) return m_vec[1];
    return m_vec[2];
  }

  Vec3<T>& operator[](int i) {
    assert(i >= 0 && i <= 2);
    if (i == 0) return m_vec[0];
    if (i == 1) return m_vec[1];
    return m_vec[2];
  }

  T trace() const;

  void zero() {
    m_vec[0].zero();
    m_vec[1].zero();
    m_vec[2].zero();
  }

  void identity() {
    m_vec[0] = Vec3<T>(1, 0, 0);
    m_vec[1] = Vec3<T>(0, 1, 0);
    m_vec[2] = Vec3<T>(0, 0, 1);
  }

  T determinant() const;
  Mat2<T> minor(const int& i, const int& j) const;
  Mat3<T> inverse() const;
  Mat3<T> transpose() const;
  T coFactor(const int& i, const int& j) const {
    Mat2<T> mi = minor(i, j);
    T C = T(pow(-1, i + 1 + j + 1)) * mi.determinant();
    return C;
  }

 private:
  Vec3<T> m_vec[3];
};

typedef Mat3<double> Mat3d;
typedef Mat3<float> Mat3f;
typedef Mat3<int> Mat3i;

template <typename T>
T Mat3<T>::trace() const {
  return m_vec[0][0] + m_vec[1][1] + m_vec[2][2];
}

template <typename T>
T Mat3<T>::determinant() const {
  double r1 =
      m_vec[0][0] * (m_vec[1][1] * m_vec[2][2] - m_vec[1][2] * m_vec[2][1]);
  double r2 =
      m_vec[0][1] * (m_vec[1][0] * m_vec[2][2] - m_vec[1][2] * m_vec[2][0]);
  double r3 =
      m_vec[0][2] * (m_vec[1][0] * m_vec[2][1] - m_vec[1][1] * m_vec[2][0]);

  return r1 - r2 + r3;
}

template <typename T>
Mat2<T> Mat3<T>::minor(const int& i, const int& j) const {
  Mat2<T> mi;

  int yy = 0;
  for (int y = 0; y < 3; y++) {
    if (y == j) continue;

    int xx = 0;
    for (int x = 0; x < 3; x++) {
      if (x == i) continue;

      mi[xx][yy] = m_vec[x][y];
      xx++;
    }

    yy++;
  }

  return mi;
}

template <typename T>
Mat3<T> Mat3<T>::inverse() const {
  Mat3<T> inv;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      inv.m_vec[j][i] = coFactor(i, j);
    }
  }

  T det = determinant();
  T invDet = 1. / det;
  inv *= invDet;
  return inv;
}

template <typename T>
Mat3<T> Mat3<T>::transpose() const {
  Mat3<T> ret;
  ret[0][0] = m_vec[0][0];
  ret[1][0] = m_vec[0][1];
  ret[2][0] = m_vec[0][2];

  ret[0][1] = m_vec[1][0];
  ret[1][1] = m_vec[1][1];
  ret[2][1] = m_vec[1][2];

  ret[0][2] = m_vec[2][0];
  ret[1][2] = m_vec[2][1];
  ret[2][2] = m_vec[2][2];

  return ret;
}

template <typename T>
Mat3<T> operator+(const Mat3<T>& m1, const Mat3<T>& m2) {
  return Mat3<T>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2]);
}

template <typename T>
Mat3<T> operator-(const Mat3<T>& m1, const Mat3<T>& m2) {
  return Mat3<T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2]);
}

template <typename T>
Mat3<T> operator*(const Mat3<T>& m1, const Mat3<T>& m2) {
  Vec3<T> row1 = m1[0];
  Vec3<T> row2 = m1[1];
  Vec3<T> row3 = m1[2];

  Vec3<T> col1 = Vec3<T>(m2[0][0], m2[1][0], m2[2][0]);
  Vec3<T> col2 = Vec3<T>(m2[0][1], m2[1][1], m2[2][1]);
  Vec3<T> col3 = Vec3<T>(m2[0][2], m2[1][2], m2[2][2]);

  Vec3<T> retRow1 = Vec3<T>(dot(row1, col1), dot(row1, col2), dot(row1, col3));
  Vec3<T> retRow2 = Vec3<T>(dot(row2, col1), dot(row2, col2), dot(row2, col3));
  Vec3<T> retRow3 = Vec3<T>(dot(row3, col1), dot(row3, col2), dot(row3, col3));

  return Mat3<T>(retRow1, retRow2, retRow3);

  /* 00 01 02
   * 10 11 12
   * 20 21 22*/
}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Mat3<T>& m) {
  out << m[0];
  out << m[1];
  out << m[2];
  return out;
}

#endif  // MAT3
