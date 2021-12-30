#include "geometry/sphere.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "tools/ray.h"

using namespace testing;

class Tsphere : public Test {
 public:
  Sphere s;
  Ray r;
};

TEST_F(Tsphere, raySphereIntersection) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 0.0f, -0.5f), Vec3f(0.0f, 0.0f, 1.0f));
  s.intersect(r);
  ASSERT_EQ(s.intersections().size(), 2);
  ASSERT_EQ(s.intersections()[0], 4.0f);
  ASSERT_EQ(s.intersections()[1], 6.0f);
}
