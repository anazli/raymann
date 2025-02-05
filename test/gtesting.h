#pragma once

#include "gtest/gtest.h"
#include "tools/tools.h"

namespace testing {

class RTest : public Test {
 public:
  void comparePoints(const Point3D& p1, const Point3D& p2) {
    EXPECT_FLOAT_EQ(p1.x(), p2.x());
    EXPECT_FLOAT_EQ(p1.y(), p2.y());
    EXPECT_FLOAT_EQ(p1.z(), p2.z());
  }
  void comparePointsApprox(const Point3D& p1, const Point3D& p2, float eps) {
    EXPECT_NEAR(p1.x(), p2.x(), eps);
    EXPECT_NEAR(p1.y(), p2.y(), eps);
    EXPECT_NEAR(p1.z(), p2.z(), eps);
  }
  void compareVectors(const Vec3D& v1, const Vec3D& v2) {
    EXPECT_FLOAT_EQ(v1.x(), v2.x());
    EXPECT_FLOAT_EQ(v1.y(), v2.y());
    EXPECT_FLOAT_EQ(v1.z(), v2.z());
  }
  void compareVectorsApprox(const Vec3D& v1, const Vec3D& v2, float eps) {
    EXPECT_NEAR(v1.x(), v2.x(), eps);
    EXPECT_NEAR(v1.y(), v2.y(), eps);
    EXPECT_NEAR(v1.z(), v2.z(), eps);
  }

 private:
};

}  // namespace testing
