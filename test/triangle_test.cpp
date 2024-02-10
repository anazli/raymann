#include "geometry/triangle.h"

#include "gtest/gtest.h"
#include "tools/ray.h"
#include "transformations/transformer.h"

using namespace testing;

class TriangleTest : public Test {
 public:
  Ray r;
  IntersectionRecord rec;
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
};

TEST_F(TriangleTest, constructingTriangle) {
  Point3f p1(0.f, 1.f, 0.f);
  Point3f p2(-1.f, 0.f, 0.f);
  Point3f p3(1.f, 0.f, 0.f);
  Triangle t({p1, p2, p3});
  comparePoints(t.point(0), p1);
  comparePoints(t.point(1), p2);
  comparePoints(t.point(2), p3);
  compareVectors(t.normal(Point3f()), Vec3f(0.f, 0.f, -1.f));
  compareVectors(t.edgeVector(0), Vec3f(-1.f, -1.f, 0.f));
  compareVectors(t.edgeVector(1), Vec3f(1.f, -1.f, 0.f));
}
