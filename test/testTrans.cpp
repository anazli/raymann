#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "tools/tools.h"

using namespace testing;

class Ttrans : public Test {
 public:
  Mat4f m;
  Vec3f v;
  Point3f p;
};

/**********************************
 *       Translation
 *********************************/

TEST_F(Ttrans, appliesTranslationToPoint) {  // TODO: better interface
  p = Point3f(-3.0f, 4.0f, 5.0f);
  m = translation(5.0f, -3.0f, 2.0f);
  Vec4f v4(p);

  v4 = m * v4;  // transformation
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), 2.0f);
  ASSERT_FLOAT_EQ(p.y(), 1.0f);
  ASSERT_FLOAT_EQ(p.z(), 7.0f);
}

TEST_F(Ttrans, appliesTranslationToPointOverloaded) {
  p = Point3f(-3.0f, 4.0f, 5.0f);
  Vec3f trans(5.0f, -3.0f, 2.0f);
  m = translation(trans);
  Vec4f v4(p);

  v4 = m * v4;  // transformation
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), 2.0f);
  ASSERT_FLOAT_EQ(p.y(), 1.0f);
  ASSERT_FLOAT_EQ(p.z(), 7.0f);
}

TEST_F(Ttrans, appliesInverseTranslationToPoint) {
  p = Point3f(-3.0f, 4.0f, 5.0f);
  m = translation(5.0f, -3.0f, 2.0f);
  m = m.inverse();
  Vec4f v4(p);

  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), -8.0f);
  ASSERT_FLOAT_EQ(p.y(), 7.0f);
  ASSERT_FLOAT_EQ(p.z(), 3.0f);
}

TEST_F(Ttrans, appliesTranslationToVector) {
  v = Vec3f(-3.0f, 4.0f, 5.0f);
  m = translation(5.0f, -3.0f, 2.0f);
  Vec4f v4(v);

  v4 = m * v4;
  Vec3f v3 = v4;

  ASSERT_TRUE(v == v3);  // translation does not affect vectors
}

/**********************************
 *       Scaling
 *********************************/

TEST_F(Ttrans, appliesScalingToPoint) {
  p = Point3f(-4.0f, 6.0f, 8.0f);
  m = scale(2.0f, 3.0f, 4.0f);
  Vec4f v4(p);

  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), -8.0f);
  ASSERT_FLOAT_EQ(p.y(), 18.0f);
  ASSERT_FLOAT_EQ(p.z(), 32.0f);
}

TEST_F(Ttrans, appliesScalingToVector) {
  v = Vec3f(-4.0f, 6.0f, 8.0f);
  m = scale(2.0f, 3.0f, 4.0f);
  Vec4f v4(v);

  v4 = m * v4;
  v = v4;

  ASSERT_FLOAT_EQ(v.x(), -8.0f);
  ASSERT_FLOAT_EQ(v.y(), 18.0f);
  ASSERT_FLOAT_EQ(v.z(), 32.0f);
}

TEST_F(Ttrans, appliesInverseScalingToVector) {  // TODO: It doesn't pass with
  v = Vec3f(-4.0f, 6.0f, 8.0f);                  // integers
  m = scale(2.0f, 3.0f, 4.0f);
  m = m.inverse();
  Vec4f v4(v);

  v4 = m * v4;
  v = v4;

  ASSERT_FLOAT_EQ(v.x(), -2.0f);
  ASSERT_FLOAT_EQ(v.y(), 2.0f);
  ASSERT_FLOAT_EQ(v.z(), 2.0f);
}

/**********************************
 *       Rotation
 *********************************/

TEST_F(Ttrans, appliesRotationXToPoint) {
  p = Point3f(0.0f, 1.0f, 0.0f);
  m = rotationX(PI / 4.0f);
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), 0.0f);
  ASSERT_FLOAT_EQ(p.y(), sqrt(2.) / 2.0f);
  ASSERT_FLOAT_EQ(p.z(), sqrt(2.) / 2.0f);
}

TEST_F(Ttrans, appliesInverseRotationXToPoint) {
  p = Point3f(0.0f, 1.0f, 0.0f);
  m = rotationX(PI / 4.0f);
  m = m.inverse();
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), 0.0f);
  ASSERT_FLOAT_EQ(p.y(), sqrt(2.) / 2.0f);
  ASSERT_FLOAT_EQ(p.z(), -sqrt(2.) / 2.0f);
}

TEST_F(Ttrans, appliesRotationYToPoint) {
  p = Point3f(0.0f, 0.0f, 1.0f);
  m = rotationY(PI / 4.0f);
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), sqrt(2.) / 2.0f);
  ASSERT_FLOAT_EQ(p.y(), 0.0f);
  ASSERT_FLOAT_EQ(p.z(), sqrt(2.) / 2.0f);
}

TEST_F(Ttrans, appliesRotationZToPoint) {
  p = Point3f(0.0f, 1.0f, 0.0f);
  m = rotationZ(PI / 4.0f);
  Vec4f v4(p);
  v4 = m * v4;
  p = v4;

  ASSERT_FLOAT_EQ(p.x(), -sqrt(2.) / 2.0f);
  ASSERT_FLOAT_EQ(p.y(), sqrt(2.) / 2.0f);
  ASSERT_FLOAT_EQ(p.z(), 0.0f);
}
