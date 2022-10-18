#include "geometry/plane.h"
#include "gtest/gtest.h"
#include "tools/ray.h"

using namespace testing;

class Tplane : public Test {
 public:
  Plane p;
  Ray r;
};

TEST_F(Tplane, normalOfPlaneIsConstantEverywhere) {
  p = Plane();
  Vec3f n1 = p.normal(Point3f(0.0f, 0.0f, 0.0f));
  Vec3f n2 = p.normal(Point3f(10.0f, 0.0f, -10.0f));
  Vec3f n3 = p.normal(Point3f(-5.0f, 0.0f, 150.0f));

  ASSERT_TRUE(n1 == Vec3f(0.0f, 1.0f, 0.0f));
  ASSERT_TRUE(n2 == Vec3f(0.0f, 1.0f, 0.0f));
  ASSERT_TRUE(n3 == Vec3f(0.0f, 1.0f, 0.0f));
}

TEST_F(Tplane, intersectRayParallelToThePlane) {
  p = Plane();
  r = Ray(Point3f(0.0f, 10.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_FALSE(p.intersect(r));
  ASSERT_TRUE(p.record().count == 0);
}

TEST_F(Tplane, intersectWithACoplanarRay) {
  p = Plane();
  r = Ray(Point3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_FALSE(p.intersect(r));
  ASSERT_TRUE(p.record().count == 0);
}

TEST_F(Tplane, rayIntersectingPlaneFromAbove) {
  p = Plane();
  r = Ray(Point3f(0.0f, 1.0f, 0.0f), Vec3f(0.0f, -1.0f, 0.0f));
  ASSERT_TRUE(p.intersect(r));
  ASSERT_TRUE(p.record().count == 1);
  ASSERT_TRUE(p.record().t_min() == 1.0f);
}

TEST_F(Tplane, rayIntersectingPlaneFromBelow) {
  p = Plane();
  r = Ray(Point3f(0.0f, -1.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
  ASSERT_TRUE(p.intersect(r));
  ASSERT_TRUE(p.record().count == 1);
  ASSERT_TRUE(p.record().t_min() == 1.0f);
}
