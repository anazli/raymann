#include "geometry/triangle.h"

#include "gtest/gtest.h"
#include "tools/ray.h"
#include "transformations/transformer.h"

using namespace testing;

class TriangleTest : public Test {
 public:
  Ray r;
  Point3f p1 = Point3f(0.f, 1.f, 0.f);
  Point3f p2 = Point3f(-1.f, 0.f, 0.f);
  Point3f p3 = Point3f(1.f, 0.f, 0.f);
  Triangle t = Triangle({p1, p2, p3});
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
  comparePoints(t.point(0), p1);
  comparePoints(t.point(1), p2);
  comparePoints(t.point(2), p3);
  compareVectors(t.normal(Point3f()), Vec3f(0.f, 0.f, -1.f));
  compareVectors(t.edgeVector(0), Vec3f(-1.f, -1.f, 0.f));
  compareVectors(t.edgeVector(1), Vec3f(1.f, -1.f, 0.f));
}

TEST_F(TriangleTest, normalVectorOfTriangle) {
  Vec3f v1 = t.normal(Point3f(0.f, 0.5f, 0.f));
  Vec3f v2 = t.normal(Point3f(1.f, 1.5f, 1.f));
  Vec3f v3 = t.normal(Point3f(0.453f, 0.5f, 6.f));
  ASSERT_TRUE(v1 == v2);
  ASSERT_TRUE(v1 == v3);
}

TEST_F(TriangleTest, intersectingRayParallelToTriangle) {
  Ray r(Point3f(0.f, -1.f, -2.f), Vec3f(0.f, 1.f, 0.f));
  ASSERT_FALSE(t.intersect(r, rec));
}

TEST_F(TriangleTest, rayMissesP1P3Edge) {
  Ray r(Point3f(1.f, 1.f, -2.f), Vec3f(0.f, 0.f, 1.f));
  ASSERT_FALSE(t.intersect(r, rec));
}

TEST_F(TriangleTest, rayMissesP1P2Edge) {
  Ray r(Point3f(-1.f, 1.f, -2.f), Vec3f(0.f, 0.f, 1.f));
  ASSERT_FALSE(t.intersect(r, rec));
}

TEST_F(TriangleTest, rayMissesP2P3Edge) {
  Ray r(Point3f(0.f, -1.f, -2.f), Vec3f(0.f, 0.f, 1.f));
  ASSERT_FALSE(t.intersect(r, rec));
}

TEST_F(TriangleTest, rayStrikesTriangle) {
  Ray r(Point3f(0.f, 0.5f, -2.f), Vec3f(0.f, 0.f, 1.f));
  ASSERT_TRUE(t.intersect(r, rec));
  ASSERT_TRUE(rec.count == 1);
  ASSERT_TRUE(rec.t_min() == 2);
}
