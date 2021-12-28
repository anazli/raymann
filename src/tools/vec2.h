#ifndef VEC2_H
#define VEC2_H

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

/*! \brief A simple 2D vector class.
 *
 *  The Vec2 class creates a 2D vector (x,y) and provides some useful operations
 *  such as dot, normalization, random vectors. It also supports all
 *  the operations such as multiplication, addition, subtraction, division etc.
 *
 */

template <class T>
class Vec2 {
 public:
  //! Constructor with default arguments. If no parameters
  //! are provided, a vector (0,0) is created. Otherwise
  //! a vector (x,y,z) is created.
  Vec2(const T& p1 = 0., const T& p2 = 0.) : m_x{p1}, m_y{p2} {}

  //! Copy Constructor. Creates a vector that is a copy
  //! of an existing vector provided as argument.
  Vec2(const Vec2<T>& v) : m_x{v.x()}, m_y{v.y()} {}

  // Getters
  T x() const { return m_x; }  //!< returns the x component.
  T y() const { return m_y; }  //!< returns the y component.

  // Setters
  void setX(const T& p) { m_x = p; }  //!< sets the x component.
  void setY(const T& p) { m_y = p; }  //!< sets the y component.

  void setXY(const T& num) {
    setX(num);
    setY(num);
  }  //!< sets the two components to a specific value.

  void setXY(const T& p1, const T& p2) {
    setX(p1);
    setY(p2);
  }  //!< Every component gets a unique value.

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

  Vec2<T> operator+() const {
    return Vec2<T>(m_x, m_y);
  };  //!< Overloaded unary (+) operator. Returns the same vector.
  Vec2<T> operator-() const {
    return Vec2<T>(-m_x, -m_y);
  }  //!< Overloaded unary (-) operator. Returns the opposite vector.

  // Prefix
  Vec2<T>& operator++();  //!< Overloaded prefix (++) operator. Returns the
                          //!< vector and increments its components by one.
  Vec2<T>& operator--();  //!< Overloaded prefix (--) operator. Returns the
                          //!< vector and decrements its components by one.
  // Postfix
  Vec2<T> operator++(int);  //!< Overloaded postfix (++) operator. Increments
                            //!< the vectors components by one and returns it.
  Vec2<T> operator--(int);  //!< Overloaded postfix (--) operator. Decrements
                            //!< the vectors components by one and returns it.

  Vec2<T>& operator+=(
      const T& num);  //!< Overloaded unary (+=) operator. Increments the
                      //!< vector's components by a value num.
  Vec2<T>& operator+=(const Vec2<T>& v);
  Vec2<T>& operator-=(
      const T& num);  //!< Overloaded unary (-=) operator. Decrements the
                      //!< vector's components by a value num.
  Vec2<T>& operator-=(const Vec2<T>& v);
  Vec2<T>& operator*=(
      const T& num);  //!< Overloaded unary (*=) operator. Multiplies the
                      //!< vector's components by a value num.

  Vec2<T>& normalize();  //!< Makes the vector a unit vector of length one.
  double length() const  //!< Returns the length of the vector.
  {
    return sqrt(x() * x() + y() * y());
  }

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
Vec2<T> operator+(const Vec2<T>& v1,
                  const Vec2<T>& v2)  //!< Overloaded binary (+) operator.
                                      //!< Addition of two vectors.
{
  return Vec2<T>(v1.x() + v2.x(), v1.y() + v2.y());
}

template <typename T>
Vec2<T> operator+(const Vec2<T>& v,
                  const T& num)  //!< Overloaded binary (+) operator. Addition
                                 //!< of !< a vector and a number.
{
  return Vec2<T>(v.x() + num, v.y() + num);
}

template <typename T>
Vec2<T> operator+(const T& num,
                  const Vec2<T>& v)  //!< Overloaded binary (+) operator.
                                     //!< Addition of a !< vector and a number.
{
  return v + num;
}

template <typename T>
Vec2<T> operator-(const Vec2<T>& v1,
                  const Vec2<T>& v2)  //!< Overloaded binary (-) operator. !<
                                      //!< Subtraction of two vectors.
{
  return Vec2<T>(v1.x() - v2.x(), v1.y() - v2.y());
}

template <typename T>
Vec2<T> operator-(const Vec2<T>& v,
                  const T& num)  //!< Overloaded binary (-) operator.
                                 //!< Subtraction !< of a vector and a number.
{
  return Vec2<T>(v.x() - num, v.y() - num);
}

template <typename T>
Vec2<T> operator-(
    const T& num,
    const Vec2<T>& v)  //!< Overloaded binary (-) operator. Subtraction !< of a
                       //!< vector and a number.
{
  return v - num;
}

template <typename T>
Vec2<T> operator*(const Vec2<T>& v1,
                  const Vec2<T>& v2)  //!< Overloaded binary (*) operator. !<
                                      //!< Multiplication of two vectors.
{
  return Vec2<T>(v1.x() * v2.x(), v1.y() * v2.y());
}

template <typename T>
Vec2<T> operator*(const Vec2<T>& v,
                  const T& num)  //!< Overloaded binary (*) operator. !<
                                 //!< Multiplication of a vector and a number.
{
  return Vec2<T>(v.x() * num, v.y() * num);
}

template <typename T>
Vec2<T> operator*(
    const T& num,
    const Vec2<T>& v)  //!< Overloaded binary (*) operator. !< Multiplication of
                       //!< a vector and a number.
{
  return v * num;
}

template <typename T>
Vec2<T> operator/(
    const Vec2<T>& v1,
    const Vec2<T>& v2)  //!< Overloaded binary (/) operator. Division of !< two
                        //!< vectors component by component.
{
  if (v2.x() == (T)0 || v2.y() == (T)0) throw "Division by zero!";
  return Vec2<T>(v1.x() / v2.x(), v1.y() / v2.y());
}

template <typename T>
Vec2<T> operator/(const Vec2<T>& v,
                  const T& num)  //!< Overloaded binary (/) operator. Division
                                 //!< of !< a vector's components by a number.
{
  T e = 1.E-30;
  return Vec2<T>(v.x() / (num + e), v.y() / (num + e));
}

template <typename T>
bool operator==(const Vec2<T>& v1,
                const Vec2<T>& v2)  //!< Overloaded binary (==) operator. Checks
                                    //!< if !< two vectors are identical.
{
  return (v1.x() == v2.x() && v1.y() == v2.y());
}

template <typename T>
bool operator!=(const Vec2<T>& v1,
                const Vec2<T>& v2)  //!< Overloaded binary (!=) operator. Checks
                                    //!< if !< two vectors are not identical.
{
  return !(v1 == v2);
}

//--------------------------------------------
// General methods
//--------------------------------------------

template <typename T>
T dot(const Vec2<T>& v1,
      const Vec2<T>& v2)  //!< The dot product of two vectors. !< Returns a
                          //!< number of type double.
{
  Vec2<T> v = v1 * v2;
  return v.x() + v.y();
}

template <typename T>
Vec2<T> getUnitVectorOf(
    const Vec2<T>& v)  //!< Gets the unit vector of a vector !< without changing
                       //!< the provided vector.
{
  return v / (v.length() + 1.E-30);
}

#endif  // VEC2_H
