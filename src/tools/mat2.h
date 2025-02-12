#pragma once

template <class T>
class Vec2;

template <class T>
class Mat2 {
 public:
  Mat2() = default;
  Mat2(T num) {
    m_vec[0].set(num);
    m_vec[1].set(num);
  }
  Mat2(const Vec2<T>& row1, const Vec2<T>& row2) {
    m_vec[0] = row1;
    m_vec[1] = row2;
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

using Mat2D = Mat2<float>;

template <typename T>
Mat2<T> operator+(const Mat2<T>& m1, const Mat2<T>& m2) {
  return Mat2<T>(m1[0] + m2[0], m1[1] + m2[1]);
}

template <typename T>
Mat2<T> operator+(const Mat2<T>& m1, T num) {
  return Mat2<T>(m1[0] + num, m1[1] + num);
}

template <typename T>
Mat2<T> operator-(const Mat2<T>& m1, const Mat2<T>& m2) {
  return Mat2<T>(m1[0] - m2[0], m1[1] - m2[1]);
}

template <typename T>
Mat2<T> operator-(const Mat2<T>& m1, T num) {
  return Mat2<T>(m1[0] - num, m1[1] - num);
}

template <typename T>
Mat2<T> operator*(const Mat2<T>& m1, T num) {
  return Mat2<T>(m1[0] * num, m1[1] * num);
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

template <typename T>
std::ostream& operator<<(std::ostream& out, const Mat2<T>& m) {
  out << "{" << m[0] << "," << m[1] << "}";
  return out;
}
