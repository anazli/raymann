#ifndef VEC3_H
#define VEC3_H

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

std::mt19937 gen(13759327u);

/*! \brief A simple 3D vector class.
 *
 *  The Vec3 class creates a 3D vector (x,y,z) and provides some useful
 * operations such as dot, cross product, normalization, random vectors. It also
 * supports all the operations such as multiplication, addition, subtraction,
 * division etc.
 *
 */

template <class T>
class Vec3 {
 public:
  //! Constructor with default arguments. If no parameters
  //! are provided, a vector (0,0,0) is created. Otherwise
  //! a vector (x,y,z) is created.
  Vec3<T>(const T& p1 = 0., const T& p2 = 0., const T& p3 = 0.)
      : m_x{p1}, m_y{p2}, m_z{p3} {}

  //! Copy Constructor. Creates a vector that is a copy
  //! of an existing vector provided as argument.
  Vec3<T>(const Vec3<T>& v) : m_x{v.x()}, m_y{v.y()}, m_z{v.z()} {}

  // Getters
  T x() const { return m_x; }  //!< returns the x component.
  T y() const { return m_y; }  //!< returns the y component.
  T z() const { return m_z; }  //!< returns the z component.

  // Setters
  void setX(const T& p) { m_x = p; }  //!< sets the x component.
  void setY(const T& p) { m_y = p; }  //!< sets the y component.
  void setZ(const T& p) { m_z = p; }  //!< sets the z component.

  void setXYZ(const T& num) {
    setX(num);
    setY(num);
    setZ(num);
  }  //!< sets the three components to a specific value.

  void setXYZ(const T& p1, const T& p2, const T& p3) {
    setX(p1);
    setY(p2);
    setZ(p3);
  }  //!< Every component gets a unique value.

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

  Vec3<T>& operator=(const Vec3<T>& v) {
    m_x = v.x();
    m_y = v.y();
    m_z = v.z();
    return *this;
  }

  Vec3<T> operator+() const {
    return Vec3<T>(m_x, m_y, m_z);
  };  //!< Overloaded unary (+) operator. Returns the same vector.
  Vec3<T> operator-() const {
    return Vec3<T>(-m_x, -m_y, -m_z);
  }  //!< Overloaded unary (-) operator. Returns the opposite vector.

  // Prefix
  Vec3<T>& operator++();  //!< Overloaded prefix (++) operator. Returns the
                          //!< vector and increments its components by one.
  Vec3<T>& operator--();  //!< Overloaded prefix (--) operator. Returns the
                          //!< vector and decrements its components by one.
  // Postfix
  Vec3<T> operator++(int);  //!< Overloaded postfix (++) operator. Increments
                            //!< the vectors components by one and returns it.
  Vec3<T> operator--(int);  //!< Overloaded postfix (--) operator. Decrements
                            //!< the vectors components by one and returns it.

  Vec3<T>& operator+=(
      const T& num);  //!< Overloaded unary (+=) operator. Increments the
                      //!< vector's components by a value num.
  Vec3<T>& operator+=(const Vec3<T>& v);
  Vec3<T>& operator-=(
      const T& num);  //!< Overloaded unary (-=) operator. Decrements the
                      //!< vector's components by a value num.
  Vec3<T>& operator-=(const Vec3<T>& v);
  Vec3<T>& operator*=(
      const T& num);  //!< Overloaded unary (*=) operator. Multiplies the
                      //!< vector's components by a value num.

  Vec3<T>& normalize();  //!< Makes the vector a unit vector of length one.
  double length() const  //!< Returns the length of the vector.
  {
    return sqrt(x() * x() + y() * y() + z() * z());
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

    return true;
  }

  void zero() {
    m_x = (T)0;
    m_y = (T)0;
    m_z = (T)0;
  }

 private:
  T m_x;
  T m_y;
  T m_z;
};

typedef Vec3<double> Vec3d;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

//--------------------------------------------
// Overloaded Member operators (input, output)
//--------------------------------------------

template <typename T>
Vec3<T>& Vec3<T>::operator++() {
  *this += 1;  // use of += member operator
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator--() {
  *this -= 1;
  return *this;
}

template <typename T>
Vec3<T> Vec3<T>::operator++(int) {
  Vec3<T> temp(m_x, m_y, m_z);
  ++(*this);  // use of prefix ++ operator
  return temp;
}

template <typename T>
Vec3<T> Vec3<T>::operator--(int) {
  Vec3<T> temp(m_x, m_y, m_z);
  --(*this);
  return temp;
}

template <typename T>
Vec3<T>& Vec3<T>::operator+=(const T& num) {
  *this = (*this) + num;  // use of binary + operator
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& v) {
  *this = (*this) + v;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator-=(const T& num) {
  *this = (*this) - num;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& v) {
  *this = (*this) - v;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator*=(const T& num) {
  *this = (*this) * num;
  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::normalize() {
  double norm = this->length();
  *this = (*this) / (norm + 1.E-30);
  return *this;
}

//--------------------------------------------
// Overloaded I/O operators (input, output)
//--------------------------------------------

template <typename T>
inline std::istream& operator>>(std::istream& in, Vec3<T>& v) {
  T x, y, z;
  in >> x >> y >> z;
  v.setXYZ(x, y, z);
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
Vec3<T> operator+(const Vec3<T>& v1,
                  const Vec3<T>& v2)  //!< Overloaded binary (+) operator.
                                      //!< Addition of two vectors.
{
  return Vec3<T>(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

template <typename T>
Vec3<T> operator+(const Vec3<T>& v,
                  const T& num)  //!< Overloaded binary (+) operator. Addition
                                 //!< of !< a vector and a number.
{
  return Vec3<T>(v.x() + num, v.y() + num, v.z() + num);
}

template <typename T>
Vec3<T> operator+(const T& num,
                  const Vec3<T>& v)  //!< Overloaded binary (+) operator.
                                     //!< Addition of a !< vector and a number.
{
  return v + num;
}

template <typename T>
Vec3<T> operator-(const Vec3<T>& v1,
                  const Vec3<T>& v2)  //!< Overloaded binary (-) operator. !<
                                      //!< Subtraction of two vectors.
{
  return Vec3<T>(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

template <typename T>
Vec3<T> operator-(const Vec3<T>& v,
                  const T& num)  //!< Overloaded binary (-) operator.
                                 //!< Subtraction !< of a vector and a number.
{
  return Vec3<T>(v.x() - num, v.y() - num, v.z() - num);
}

template <typename T>
Vec3<T> operator-(
    const T& num,
    const Vec3<T>& v)  //!< Overloaded binary (-) operator. Subtraction !< of a
                       //!< vector and a number.
{
  return v - num;
}

template <typename T>
Vec3<T> operator*(const Vec3<T>& v1,
                  const Vec3<T>& v2)  //!< Overloaded binary (*) operator. !<
                                      //!< Multiplication of two vectors.
{
  return Vec3<T>(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

template <typename T>
Vec3<T> operator*(const Vec3<T>& v,
                  const T& num)  //!< Overloaded binary (*) operator.
                                 //!< Multiplication of a vector and a number.
{
  return Vec3<T>(v.x() * num, v.y() * num, v.z() * num);
}

template <typename T>
Vec3<T> operator*(
    const T& num,
    const Vec3<T>& v)  //!< Overloaded binary (*) operator.
                       //!< Multiplication of a vector and a number.
{
  return v * num;
}

template <typename T>
Vec3<T> operator/(
    const Vec3<T>& v1,
    const Vec3<T>& v2)  //!< Overloaded binary (/) operator. Division of
                        //!< two vectors component by component.
{
  if (v2.x() == 0. || v2.y() == 0. || v2.z() == 0.) throw "Division by zero!";
  return Vec3<T>(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}

template <typename T>
Vec3<T> operator/(const Vec3<T>& v,
                  const T& num)  //!< Overloaded binary (/) operator. Division
                                 //!< of a vector's components by a number.
{
  double e = 1.E-30;
  return Vec3<T>(v.x() / (num + e), v.y() / (num + e), v.z() / (num + e));
}

template <typename T>
bool operator==(const Vec3<T>& v1,
                const Vec3<T>& v2)  //!< Overloaded binary (==) operator. Checks
                                    //!< if two vectors are identical.
{
  return (v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z());
}

template <typename T>
bool operator!=(const Vec3<T>& v1,
                const Vec3<T>& v2)  //!< Overloaded binary (!=) operator. Checks
                                    //!< if two vectors are not identical.
{
  return !(v1 == v2);
}

template <typename T>
T dot(const Vec3<T>& v1,
      const Vec3<T>& v2)  //!< The dot product of two vectors.
                          //!< Returns a number of type double.
{
  Vec3<T> v = v1 * v2;
  return v.x() + v.y() + v.z();
}

template <typename T>
Vec3<T> cross(
    const Vec3<T>& v1,
    const Vec3<T>& v2)  //!< The cross product of two vectors. Returns a
                        //!< vector perpendicular to the surface of them.
{
  T x = v1.y() * v2.z() - v1.z() * v2.y();
  T y = v1.z() * v2.x() - v1.x() * v2.z();
  T z = v1.x() * v2.y() - v1.y() * v2.x();
  return Vec3<T>(x, y, z);
}

template <typename T>
Vec3<T> getUnitVectorOf(
    const Vec3<T>& v)  //!< Gets the unit vector of a vector
                       //!< without changing the provided vector.
{
  return v / (v.length() + 1.E-30);
}

template <typename T>
Vec3<T> randomVector(
    const T& a = 0.,
    const T& b = 1.)  //!< Returns a vector with uniformly random
                      //!< components in the interval [a,b].
{
  std::uniform_real_distribution<T> rand(a, b);
  return Vec3<T>(rand(gen), rand(gen), rand(gen));
}

template <typename T>
Vec3<T>
randomVectorOnUnitSphere()  //!< Returns a random vector on a unit sphere.
{
  std::uniform_real_distribution<T> rand(0., 1.);
  T xi1, xi2, dsq = 2;
  while (dsq >= 1.) {
    xi1 = 1. - 2. * rand(gen);
    xi2 = 1. - 2. * rand(gen);
    dsq = xi1 * xi1 + xi2 * xi2;
  }

  T ranh = 2. * sqrt(1. - dsq);
  T dmx = xi1 * ranh;
  T dmy = xi2 * ranh;
  T dmz = 1. - 2. * dsq;

  Vec3<T> ret(dmx, dmy, dmz);
  return ret;
}

#endif  // VEC3_H
