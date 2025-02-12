#pragma once

template <class T>
class Vec3;

template <class T>
class Mat2;

template <class T>
class Mat3 {
 public:
  Mat3() {
    m_vec[0] = Vec3<T>(T{1}, T{0}, T{0});
    m_vec[1] = Vec3<T>(T{0}, T{1}, T{0});
    m_vec[2] = Vec3<T>(T{0}, T{0}, T{1});
  }
  Mat3(T num) {
    m_vec[0].set(num);
    m_vec[1].set(num);
    m_vec[2].set(num);
  }
  Mat3(const Vec3<T>& row1, const Vec3<T>& row2, const Vec3<T>& row3) {
    m_vec[0] = row1;
    m_vec[1] = row2;
    m_vec[2] = row3;
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
  Mat2<T> minor(int i, int j) const;
  Mat3<T> inverse() const;
  Mat3<T> transpose() const;
  T coFactor(int i, int j) const {
    return T(pow(-1., i + 1. + j + 1.)) * minor(i, j).determinant();
  }

 private:
  Vec3<T> m_vec[3];
};

using Mat3D = Mat3<float>;

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
Mat2<T> Mat3<T>::minor(int i, int j) const {
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
  inv = inv * invDet;
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
Mat3<T> operator+(const Mat3<T>& m1, T num) {
  return Mat3<T>(m1[0] + num, m1[1] + num, m1[2] + num);
}

template <typename T>
Mat3<T> operator-(const Mat3<T>& m1, const Mat3<T>& m2) {
  return Mat3<T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2]);
}

template <typename T>
Mat3<T> operator-(const Mat3<T>& m1, T num) {
  return Mat3<T>(m1[0] - num, m1[1] - num, m1[2] - num);
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
Mat3<T> operator*(const Mat3<T>& m1, T num) {
  return Mat3<T>(m1[0] * num, m1[1] * num, m1[2] * num);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Mat3<T>& m) {
  out << "{" << m[0] << "," << m[1] << "," << m[2] << "}";
  return out;
}
