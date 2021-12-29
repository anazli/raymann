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

TEST_F(Ttrans, appliesTranslationToPoint) {  // TODO: better interface
  p = Point3f(-3, 4, 5);
  m = translation(5.0f, -3.0f, 2.0f);
  Vec4f v4(p);

  v4 = m * v4;  // transformation
  p = v4;

  ASSERT_EQ(p.x(), 2);
  ASSERT_EQ(p.y(), 1);
  ASSERT_EQ(p.z(), 7);
}

TEST_F(Ttrans, appliesTranslationToPointOverloaded) {
  p = Point3f(-3, 4, 5);
  Vec3f trans(5, -3, 2);
  m = translation(trans);
  Vec4f v4(p);

  v4 = m * v4;  // transformation
  p = v4;

  ASSERT_EQ(p.x(), 2);
  ASSERT_EQ(p.y(), 1);
  ASSERT_EQ(p.z(), 7);
}

TEST_F(Ttrans, appliesInverseTranslationToPoint) {
  p = Point3f(-3, 4, 5);
  m = translation(5.0f, -3.0f, 2.0f);
  m = m.inverse();
  Vec4f v4(p);

  v4 = m * v4;
  p = v4;

  ASSERT_EQ(p.x(), -8);
  ASSERT_EQ(p.y(), 7);
  ASSERT_EQ(p.z(), 3);
}

TEST_F(Ttrans, appliesTranslationToVector) {
  v = Vec3f(-3, 4, 5);
  m = translation(5.0f, -3.0f, 2.0f);
  Vec4f v4(v);

  v4 = m * v4;
  Vec3f v3 = v4;

  ASSERT_TRUE(v == v3);  // translation does not affect vectors
}

TEST_F(Ttrans, appliesScalingToPoint) {
  p = Point3f(-4, 6, 8);
  m = scale(2.0f, 3.0f, 4.0f);
  Vec4f v4(p);

  v4 = m * v4;
  p = v4;

  ASSERT_EQ(p.x(), -8);
  ASSERT_EQ(p.y(), 18);
  ASSERT_EQ(p.z(), 32);
}

TEST_F(Ttrans, appliesScalingToVector) {
  v = Vec3f(-4, 6, 8);
  m = scale(2.0f, 3.0f, 4.0f);
  Vec4f v4(v);

  v4 = m * v4;
  v = v4;

  ASSERT_EQ(v.x(), -8);
  ASSERT_EQ(v.y(), 18);
  ASSERT_EQ(v.z(), 32);
}

TEST_F(Ttrans, appliesInverseScalingToVector) {  // TODO: It doesn't pass with
  v = Vec3f(-4, 6, 8);                           // integers
  m = scale(2.0f, 3.0f, 4.0f);
  m = m.inverse();
  Vec4f v4(v);

  v4 = m * v4;
  v = v4;

  ASSERT_EQ(v.x(), -2);
  ASSERT_EQ(v.y(), 2);
  ASSERT_EQ(v.z(), 2);
}
