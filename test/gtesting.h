#pragma once

#include "gtest/gtest.h"
#include "math_utils/math_utils.h"

namespace testing {

class RTest : public Test {
 public:
  void comparePoints(const Point3f& p1, const Point3f& p2) {
    EXPECT_FLOAT_EQ(p1.x(), p2.x());
    EXPECT_FLOAT_EQ(p1.y(), p2.y());
    EXPECT_FLOAT_EQ(p1.z(), p2.z());
  }
  void comparePointsApprox(const Point3f& p1, const Point3f& p2, float eps) {
    EXPECT_NEAR(p1.x(), p2.x(), eps);
    EXPECT_NEAR(p1.y(), p2.y(), eps);
    EXPECT_NEAR(p1.z(), p2.z(), eps);
  }
  void compareVectors(const Vec3f& v1, const Vec3f& v2) {
    EXPECT_FLOAT_EQ(v1.x(), v2.x());
    EXPECT_FLOAT_EQ(v1.y(), v2.y());
    EXPECT_FLOAT_EQ(v1.z(), v2.z());
  }
  void compareVectors(const Normal3f& n, const Vec3f& v) {
    EXPECT_FLOAT_EQ(n.x(), v.x());
    EXPECT_FLOAT_EQ(n.y(), v.y());
    EXPECT_FLOAT_EQ(n.z(), v.z());
  }
  void compareVectors(const Normal3f& n, const Normal3f& v) {
    EXPECT_FLOAT_EQ(n.x(), v.x());
    EXPECT_FLOAT_EQ(n.y(), v.y());
    EXPECT_FLOAT_EQ(n.z(), v.z());
  }
  void compareVectorsApprox(const Vec3f& v1, const Vec3f& v2, float eps) {
    EXPECT_NEAR(v1.x(), v2.x(), eps);
    EXPECT_NEAR(v1.y(), v2.y(), eps);
    EXPECT_NEAR(v1.z(), v2.z(), eps);
  }
  void compareVectorsApprox(const Normal3f& n, const Vec3f& v, float eps) {
    EXPECT_NEAR(n.x(), v.x(), eps);
    EXPECT_NEAR(n.y(), v.y(), eps);
    EXPECT_NEAR(n.z(), v.z(), eps);
  }

 private:
};

}  // namespace testing
