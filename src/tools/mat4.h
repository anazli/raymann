#pragma once

#include "application/error.h"

template <class T>
class Vec4;

template <class T>
class Vec3;

template <class T>
class Point3;

template <class T>
class Mat2;

template <class T>
class Mat3;

template <class T>
class Mat4 {
 public:
  Mat4() {
    m_vec[0] = Vec4<T>(T{1}, T{0}, T{0}, T{0});
    m_vec[1] = Vec4<T>(T{0}, T{1}, T{0}, T{0});
    m_vec[2] = Vec4<T>(T{0}, T{0}, T{1}, T{0});
    m_vec[3] = Vec4<T>(T{0}, T{0}, T{0}, T{1});
  }
  Mat4(T num) {
    m_vec[0].set(num);
    m_vec[1].set(num);
    m_vec[2].set(num);
    m_vec[3].set(num);
  }
  Mat4(const Vec4<T>& row1, const Vec4<T>& row2, const Vec4<T>& row3,
       const Vec4<T>& row4) {
    m_vec[0] = row1;
    m_vec[1] = row2;
    m_vec[2] = row3;
    m_vec[3] = row4;
  }

  auto operator<=>(const Mat4<T>&) const = default;

  Vec4<T> operator[](int i) const {
    assert(i >= 0 && i <= 3);
    if (i == 0) return m_vec[0];
    if (i == 1) return m_vec[1];
    if (i == 2) return m_vec[2];
    return m_vec[3];
  }

  Vec4<T>& operator[](int i) {
    assert(i >= 0 && i <= 3);
    if (i == 0) return m_vec[0];
    if (i == 1) return m_vec[1];
    if (i == 2) return m_vec[2];
    return m_vec[3];
  }

  T trace() const;

  void zero() {
    m_vec[0].zero();
    m_vec[1].zero();
    m_vec[2].zero();
    m_vec[3].zero();
  }

  void identity() {
    m_vec[0] = Vec4<T>(1, 0, 0, 0);
    m_vec[1] = Vec4<T>(0, 1, 0, 0);
    m_vec[2] = Vec4<T>(0, 0, 1, 0);
    m_vec[3] = Vec4<T>(0, 0, 0, 1);
  }

  T determinant() const;
  Mat3<T> minor(int i, int j) const;
  Mat4<T> inverse() const;
  Mat4<T> transpose() const;
  T coFactor(int i, int j) const {
    return T(pow(-1., i + 1. + j + 1.)) * minor(i, j).determinant();
  }

  void Orient(const Vec3<T>& pos, const Vec3<T>& fwd, const Vec3<T>& up);
  void LookAt(const Vec3<T>& pos, const Vec3<T>& lookAt, const Vec3<T>& up);

 private:
  Vec4<T> m_vec[4];
};

using Mat4D = Mat4<float>;

template <typename T>
T Mat4<T>::trace() const {
  return m_vec[0][0] + m_vec[1][1] + m_vec[2][2] + m_vec[3][3];
}

template <typename T>
T Mat4<T>::determinant() const {
  T det = 0;
  for (int j = 0; j < 4; ++j) {
    Mat3<T> mi = minor(0, j);
    det += m_vec[0][j] * coFactor(0, j);
  }
  return det;
}

template <typename T>
Mat3<T> Mat4<T>::minor(int i, int j) const {
  Mat3<T> mi;
  int yy = 0;
  for (int y = 0; y < 4; y++) {
    if (y == j) continue;
    int xx = 0;
    for (int x = 0; x < 4; x++) {
      if (x == i) continue;
      mi[xx][yy] = m_vec[x][y];
      xx++;
    }
    yy++;
  }
  return mi;
}

template <typename T>
Mat4<T> Mat4<T>::inverse() const {
  T det = determinant();
  APP_ASSERT(det != 0, "Matrix is not invertible!");
  Mat4<T> inv;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      T c = coFactor(i, j);
      inv.m_vec[j][i] = coFactor(i, j) / det;
    }
  }
  return inv;
}

template <typename T>
Mat4<T> Mat4<T>::transpose() const {
  Mat4<T> ret;
  ret[0][0] = m_vec[0][0];
  ret[1][0] = m_vec[0][1];
  ret[2][0] = m_vec[0][2];
  ret[3][0] = m_vec[0][3];

  ret[0][1] = m_vec[1][0];
  ret[1][1] = m_vec[1][1];
  ret[2][1] = m_vec[1][2];
  ret[3][1] = m_vec[1][3];

  ret[0][2] = m_vec[2][0];
  ret[1][2] = m_vec[2][1];
  ret[2][2] = m_vec[2][2];
  ret[3][2] = m_vec[2][3];

  ret[0][3] = m_vec[3][0];
  ret[1][3] = m_vec[3][1];
  ret[2][3] = m_vec[3][2];
  ret[3][3] = m_vec[3][3];

  return ret;
}

template <typename T>
void Mat4<T>::Orient(const Vec3<T>& pos, const Vec3<T>& fwd,
                     const Vec3<T>& up) {
  Vec3<T> left = up.cross(fwd);
  m_vec[0] = Vec4<T>(fwd.x(), left.x(), up.x(), pos.x());
  m_vec[1] = Vec4<T>(fwd.y(), left.y(), up.y(), pos.y());
  m_vec[2] = Vec4<T>(fwd.z(), left.z(), up.z(), pos.z());
  m_vec[3] = Vec4<T>(T{0}, T{0}, T{0}, T{1});
}
template <typename T>
void Mat4<T>::LookAt(const Vec3<T>& pos, const Vec3<T>& lookAt,
                     const Vec3<T>& up) {
  Vec3<T> fwd = pos - lookAt;
  fwd.normalize();

  Vec3<T> right = up.cross(fwd);
  right.normalize();

  up = fwd.cross(right);
  up.normalize();

  m_vec[0] = Vec4<T>(right.x(), right.y(), right.z(), -dot(pos, right));
  m_vec[1] = Vec4<T>(up.x(), up.y(), up.z(), -dot(pos, up));
  m_vec[2] = Vec4<T>(fwd.x(), fwd.y(), fwd.z(), -dot(pos, fwd));
  m_vec[3] = Vec4<T>(T{0}, T{0}, T{0}, T{1});
}

template <typename T>
Mat4<T> operator+(const Mat4<T>& m1, const Mat4<T>& m2) {
  return Mat4<T>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2], m1[3] + m2[3]);
}

template <typename T>
Mat4<T> operator+(const Mat4<T>& m1, T num) {
  return Mat4<T>(m1[0] + num, m1[1] + num, m1[2] + num, m1[3] + num);
}

template <typename T>
Mat4<T> operator-(const Mat4<T>& m1, const Mat4<T>& m2) {
  return Mat4<T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2], m1[3] - m2[3]);
}

template <typename T>
Mat4<T> operator-(const Mat4<T>& m1, T num) {
  return Mat4<T>(m1[0] - num, m1[1] - num, m1[2] - num, m1[3] - num);
}

template <typename T>
Mat4<T> operator*(const Mat4<T>& m1, const Mat4<T>& m2) {
  Vec4<T> row1 = m1[0];
  Vec4<T> row2 = m1[1];
  Vec4<T> row3 = m1[2];
  Vec4<T> row4 = m1[3];

  Vec4<T> col1 = Vec4<T>(m2[0][0], m2[1][0], m2[2][0], m2[3][0]);
  Vec4<T> col2 = Vec4<T>(m2[0][1], m2[1][1], m2[2][1], m2[3][1]);
  Vec4<T> col3 = Vec4<T>(m2[0][2], m2[1][2], m2[2][2], m2[3][2]);
  Vec4<T> col4 = Vec4<T>(m2[0][3], m2[1][3], m2[2][3], m2[3][3]);

  Vec4<T> retRow1 = Vec4<T>(dot(row1, col1), dot(row1, col2), dot(row1, col3),
                            dot(row1, col4));
  Vec4<T> retRow2 = Vec4<T>(dot(row2, col1), dot(row2, col2), dot(row2, col3),
                            dot(row2, col4));
  Vec4<T> retRow3 = Vec4<T>(dot(row3, col1), dot(row3, col2), dot(row3, col3),
                            dot(row3, col4));
  Vec4<T> retRow4 = Vec4<T>(dot(row4, col1), dot(row4, col2), dot(row4, col3),
                            dot(row4, col4));

  return Mat4<T>(retRow1, retRow2, retRow3, retRow4);

  /* 00 01 02 03
   * 10 11 12 13
   * 20 21 22 23
   * 30 31 32 33*/
}

template <typename T>
Vec4<T> operator*(const Mat4<T>& m, const Vec4<T>& v) {
  Vec4<T> ret;
  ret[0] = dot(m[0], v);
  ret[1] = dot(m[1], v);
  ret[2] = dot(m[2], v);
  ret[3] = dot(m[3], v);
  return ret;
}

template <typename T>
Mat4<T> operator*(const Mat4<T>& m1, T num) {
  return Mat4<T>(m1[0] * num, m1[1] * num, m1[2] * num, m1[3] * num);
}

template <typename T>
Mat4<T> translation(T x, T y, T z) {
  Mat4<T> ret;
  ret.identity();
  ret[0][3] = x;
  ret[1][3] = y;
  ret[2][3] = z;
  return ret;
}

template <typename T>
Mat4<T> translation(const Vec3<T>& v) {
  Mat4<T> ret;
  ret.identity();
  ret[0][3] = v.x();
  ret[1][3] = v.y();
  ret[2][3] = v.z();
  return ret;
}

template <typename T>
Mat4<T> scale(T x, T y, T z) {
  Mat4<T> ret;
  ret.identity();
  ret[0][0] = x;
  ret[1][1] = y;
  ret[2][2] = z;
  return ret;
}

template <typename T>
Mat4<T> scale(const Vec3<T>& v) {
  Mat4<T> ret;
  ret.identity();
  ret[0][0] = v.x();
  ret[1][1] = v.y();
  ret[2][2] = v.z();
  return ret;
}

// TODO: floating point errors ~ E-8

template <typename T>
Mat4<T> rotationOverX(T rad) {
  Mat4<T> ret;
  ret.identity();
  ret[1][1] = static_cast<T>(cos(rad));
  ret[1][2] = -static_cast<T>(sin(rad));
  ret[2][1] = static_cast<T>(sin(rad));
  ret[2][2] = static_cast<T>(cos(rad));
  return ret;
}

template <typename T>
Mat4<T> rotationOverY(T rad) {
  Mat4<T> ret;
  ret.identity();
  ret[0][0] = static_cast<T>(cos(rad));
  ret[0][2] = static_cast<T>(sin(rad));
  ret[2][0] = -static_cast<T>(sin(rad));
  ret[2][2] = static_cast<T>(cos(rad));
  return ret;
}

template <typename T>
Mat4<T> rotationOverZ(T rad) {
  Mat4<T> ret;
  ret.identity();
  ret[0][0] = static_cast<T>(cos(rad));
  ret[0][1] = -static_cast<T>(sin(rad));
  ret[1][0] = static_cast<T>(sin(rad));
  ret[1][1] = static_cast<T>(cos(rad));
  return ret;
}

template <typename T>
Mat4<T> view_transform(const Point3<T>& from, const Point3<T>& to,
                       const Vec3<T>& up) {
  Vec3<T> forward = getUnitVectorOf(to - from);
  Vec3<T> up_norm = getUnitVectorOf(up);
  Vec3<T> left = cross(forward, up_norm);
  Vec3<T> up_res = cross(left, forward);

  Mat4D orientation =
      Mat4D(Vec4<T>(left.x(), left.y(), left.z(), 0.0f),
            Vec4<T>(up_res.x(), up_res.y(), up_res.z(), 0.0f),
            Vec4<T>(-forward.x(), -forward.y(), -forward.z(), 0.0f),
            Vec4<T>(0.0f, 0.0f, 0.0f, 1.0f));

  return orientation * translation(-from.x(), -from.y(), -from.z());
}

// TODO: Shearing

template <typename T>
std::ostream& operator<<(std::ostream& out, const Mat4<T>& m) {
  out << "{" << m[0] << "," << m[1] << "," << m[2] << "," << m[3] << "}";
  return out;
}