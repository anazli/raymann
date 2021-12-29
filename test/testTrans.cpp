#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "tools/tools.h"

using namespace testing;

class Ttrans : public Test {
 public:
  Mat4i m;
  Vec3i v;
  Point3i p;
};

TEST_F(Ttrans, appliesTranslationToPoint) {  // TODO: better interface
  p = Point3i(-3, 4, 5);
  m = translation(5, -3, 2);
  Vec4i v4(p);

  v4 = m * v4;  // transformation
  p = v4;

  ASSERT_EQ(p.x(), 2);
  ASSERT_EQ(p.y(), 1);
  ASSERT_EQ(p.z(), 7);
}

TEST_F(Ttrans, appliesInverseTranslationToPoint) {
  p = Point3i(-3, 4, 5);
  m = translation(5, -3, 2);
  m = m.inverse();
  Vec4i v4(p);

  v4 = m * v4;
  p = v4;

  ASSERT_EQ(p.x(), -8);
  ASSERT_EQ(p.y(), 7);
  ASSERT_EQ(p.z(), 3);
}

TEST_F(Ttrans, appliesTranslationToVector) {
  v = Vec3i(-3, 4, 5);
  m = translation(5, -3, 2);
  Vec4i v4(v);

  v4 = m * v4;
  Vec3i v3 = v4;

  ASSERT_TRUE(v == v3);  // translation does not affect vectors
}
