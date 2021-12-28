#ifndef VEC4_H
#define VEC4_H

#include <cassert>
#include <iostream>
#include <random>

/*! \brief A simple 4D vector class.
 *
 *  The Vec4 class creates a 4D vector (x,y,z,w) and provides some useful
 * operations such as dot, cross product, normalization, random vectors. It also
 * supports all the operations such as multiplication, addition, subtraction,
 * division etc.
 *
 */

template <class T>
class Vec4 {
 public:
  //! Constructor with default arguments. If no parameters
  //! are provided, a vector (0,0,0,0) is created. Otherwise
  //! a vector (x,y,z) is created.
  Vec4<T>(const T& p1 = 0., const T& p2 = 0., const T& p3 = 0.,
          const T& p4 = 0.)
      : m_x{p1}, m_y{p2}, m_z{p3}, m_w{p4} {}

  //! Copy Constructor. Creates a vector that is a copy
  //! of an existing vector provided as argument.
  Vec4<T>(const Vec4<T>& v) : m_x{v.x()}, m_y{v.y()}, m_z{v.z()}, m_w{v.w()} {}

  // Getters
  T x() const { return m_x; }  //!< returns the x component.
  T y() const { return m_y; }  //!< returns the y component.
  T z() const { return m_z; }  //!< returns the z component.
  T w() const { return m_w; }  //!< returns the w component.

  // Setters
  void setX(const T& p) { m_x = p; }  //!< sets the x component.
  void setY(const T& p) { m_y = p; }  //!< sets the y component.
  void setZ(const T& p) { m_z = p; }  //!< sets the z component.
  void setW(const T& p) { m_w = p; }  //!< sets the w component.

  void setXYZW(const T& num) {
    setX(num);
    setY(num);
    setZ(num);
    setW(num);
  }  //!< sets the four components to a specific value.

  void setXYZW(const T& p1, const T& p2, const T& p3, const T& p4) {
    setX(p1);
    setY(p2);
    setZ(p3);
    setW(p4);
  }  //!< Every component gets a unique value.

  T operator[](int i) const {
    assert(i >= 0 && i <= 3);
    if (i == 0) return m_x;
    if (i == 1) return m_y;
    if (i == 2) return m_z;
    return m_w;
  }

  T& operator[](int i) {
    assert(i >= 0 && i <= 3);
    if (i == 0) return m_x;
    if (i == 1) return m_y;
    if (i == 2) return m_z;
    return m_w;
  }

  Vec4<T>& operator=(const Vec4<T>& v) {
    m_x = v.x();
    m_y = v.y();
    m_z = v.z();
    m_w = v.w();
    return *this;
  }

  Vec4<T> operator+() const {
    return Vec4<T>(m_x, m_y, m_z, m_w);
  };  //!< Overloaded unary (+) operator. Returns the same vector.
  Vec4<T> operator-() const {
    return Vec4<T>(-m_x, -m_y, -m_z, -m_w);
  }  //!< Overloaded unary (-) operator. Returns the opposite vector.

  // Prefix
  Vec4<T>& operator++();  //!< Overloaded prefix (++) operator. Returns the
                          //!< vector and increments its components by one.
  Vec4<T>& operator--();  //!< Overloaded prefix (--) operator. Returns the
                          //!< vector and decrements its components by one.
  // Postfix
  Vec4<T> operator++(int);  //!< Overloaded postfix (++) operator. Increments
                            //!< the vectors components by one and returns it.
  Vec4<T> operator--(int);  //!< Overloaded postfix (--) operator. Decrements
                            //!< the vectors components by one and returns it.

  Vec4<T>& operator+=(
      const T& num);  //!< Overloaded unary (+=) operator. Increments the
                      //!< vector's components by a value num.
  Vec4<T>& operator+=(const Vec4<T>& v);
  Vec4<T>& operator-=(
      const T& num);  //!< Overloaded unary (-=) operator. Decrements the
                      //!< vector's components by a value num.
  Vec4<T>& operator-=(const Vec4<T>& v);
  Vec4<T>& operator*=(
      const T& num);  //!< Overloaded unary (*=) operator. Multiplies the
                      //!< vector's components by a value num.

  Vec4<T>& normalize();  //!< Makes the vector a unit vector of length one.
  double length() const  //!< Returns the length of the vector.
  {
    return sqrt(x() * x() + y() * y() + z() * z() + w() * w());
  }

  bool isValid() {
    if (m_x * 0. != m_x * 0.) {
      return false;
    }
    if (m_y * 0. != m_y * 0.) {
      return false;
    }
    if (m_z * 0. != m_z * 0.) {
      return false;
    }
    if (m_w * 0. != m_w * 0.) {
      return false;
    }

    return true;
  }

  void zero() {
    m_x = (T)0;
    m_y = (T)0;
    m_z = (T)0;
    m_w = (T)0;
  }

 private:
  T m_x;
  T m_y;
  T m_z;
  T m_w;
};

typedef Vec4<double> Vec4d;
typedef Vec4<float> Vec4f;
typedef Vec4<int> Vec4i;

//--------------------------------------------
// Overloaded Member operators
//--------------------------------------------

template <typename T>
Vec4<T>& Vec4<T>::operator++() {
  *this += 1;  // use of += member operator
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator--() {
  *this -= 1;
  return *this;
}

template <typename T>
Vec4<T> Vec4<T>::operator++(int) {
  Vec4<T> temp(m_x, m_y, m_z, m_w);
  ++(*this);  // use of prefix ++ operator
  return temp;
}

template <typename T>
Vec4<T> Vec4<T>::operator--(int) {
  Vec4<T> temp(m_x, m_y, m_z, m_w);
  --(*this);
  return temp;
}

template <typename T>
Vec4<T>& Vec4<T>::operator+=(const T& num) {
  *this = (*this) + num;  // use of binary + operator
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& v) {
  *this = (*this) + v;
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator-=(const T& num) {
  *this = (*this) - num;
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& v) {
  *this = (*this) - v;
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator*=(const T& num) {
  *this = (*this) * num;
  return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::normalize() {
  double norm = this->length();
  *this = (*this) / (norm + 1.E-30);
  return *this;
}

//--------------------------------------------
// Overloaded I/O operators (input, output)
//--------------------------------------------

template <typename T>
std::istream& operator>>(std::istream& in, Vec4<T>& v) {
  T x, y, z, w;
  in >> x >> y >> z >> w;
  v.setXYZW(x, y, z, w);
  return in;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Vec4<T>& v) {
  out << "(" << v.x() << "," << v.y() << "," << v.z() << "," << v.w() << ")";
  return out;
}

template <typename T>
Vec4<T> operator+(const Vec4<T>& v1,
                  const Vec4<T>& v2)  //!< Overloaded binary (+) operator.
                                      //!< Addition of two vectors.
{
  return Vec4<T>(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z(),
                 v1.w() + v2.w());
}

template <typename T>
Vec4<T> operator+(const Vec4<T>& v,
                  const T& num)  //!< Overloaded binary (+) operator. Addition
                                 //!< of a vector and a number.
{
  return Vec4<T>(v.x() + num, v.y() + num, v.z() + num, v.w() + num);
}

template <typename T>
Vec4<T> operator+(const T& num,
                  const Vec4<T>& v)  //!< Overloaded binary (+) operator.
                                     //!< Addition of a vector and a number.
{
  return v + num;
}

template <typename T>
Vec4<T> operator-(const Vec4<T>& v1,
                  const Vec4<T>& v2)  //!< Overloaded binary (-) operator.
                                      //!< Subtraction of two vectors.
{
  return Vec4<T>(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z(),
                 v1.w() - v2.w());
}

template <typename T>
Vec4<T> operator-(const Vec4<T>& v,
                  const T& num)  //!< Overloaded binary (-) operator.
                                 //!< Subtraction of a vector and a number.
{
  return Vec4<T>(v.x() - num, v.y() - num, v.z() - num, v.w() - num);
}

template <typename T>
Vec4<T> operator-(const T& num,
                  const Vec4<T>& v)  //!< Overloaded binary (-) operator.
                                     //!< Subtraction of a vector and a number.
{
  return v - num;
}

template <typename T>
Vec4<T> operator*(const Vec4<T>& v1,
                  const Vec4<T>& v2)  //!< Overloaded binary (*) operator.
                                      //!< Multiplication of two vectors.
{
  return Vec4<T>(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z(),
                 v1.w() * v2.w());
}

template <typename T>
Vec4<T> operator*(const Vec4<T>& v,
                  const T& num)  //!< Overloaded binary (*) operator.
                                 //!< Multiplication of a vector and a number.
{
  return Vec4<T>(v.x() * num, v.y() * num, v.z() * num, v.w() * num);
}

template <typename T>
Vec4<T> operator*(
    const T& num,
    const Vec4<T>& v)  //!< Overloaded binary (*) operator.
                       //!< Multiplication of a vector and a number.
{
  return v * num;
}

template <typename T>
Vec4<T> operator/(
    const Vec4<T>& v1,
    const Vec4<T>& v2)  //!< Overloaded binary (/) operator. Division of
                        //!< two vectors component by component.
{
  if (v2.x() == 0. || v2.y() == 0. || v2.z() == 0. || v2.w() == 0.)
    throw "Division by zero!";
  return Vec4<T>(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z(),
                 v1.w() / v2.w());
}

template <typename T>
Vec4<T> operator/(const Vec4<T>& v,
                  const T& num)  //!< Overloaded binary (/) operator. Division
                                 //!< of a vector's components by a number.
{
  double e = 1.E-30;
  return Vec4<T>(v.x() / (num + e), v.y() / (num + e), v.z() / (num + e),
                 v.w() / (num + e));
}

template <typename T>
bool operator==(const Vec4<T>& v1,
                const Vec4<T>& v2)  //!< Overloaded binary (==) operator. Checks
                                    //!< if two vectors are identical.
{
  return (v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z() &&
          v1.w() == v2.w());
}

template <typename T>
bool operator!=(const Vec4<T>& v1,
                const Vec4<T>& v2)  //!< Overloaded binary (!=) operator. Checks
                                    //!< if two vectors are not identical.
{
  return !(v1 == v2);
}

template <typename T>
double dot(const Vec4<T>& v1,
           const Vec4<T>& v2)  //!< The dot product of two vectors.
                               //!< Returns a number of type double.
{
  Vec4<T> v = v1 * v2;
  return v.x() + v.y() + v.z() + v.w();
}

template <typename T>
Vec4<T> getUnitVectorOf(
    const Vec4<T>& v)  //!< Gets the unit vector of a vector
                       //!< without changing the provided vector.
{
  return v / (v.length() + 1.E-30);
}

#endif  // VEC4_H
