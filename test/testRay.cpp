#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "tools/ray.h"
#include "tools/tools.h"

using namespace testing;

class Tray : public Test {
 public:
  Ray r;
  Point3f ori;
  Vec3f dir;
};

TEST_F(Tray, createsRay) {
  ori = Point3f(1, 2, 3);
  dir = Vec3f(4, 5, 6);
  r = Ray(ori, dir);

  ASSERT_TRUE(r.origin() == ori);
  ASSERT_TRUE(r.direction() == dir);
}

TEST_F(Tray, computesPositionAtT) {
  ori = Point3f(2, 3, 4);
  dir = Vec3f(1, 0, 0);
  r = Ray(ori, dir);

  ASSERT_TRUE(r.position(0) == ori);
  ASSERT_TRUE(r.position(1) == Point3f(3, 3, 4));
  ASSERT_TRUE(r.position(-1) == Point3f(1, 3, 4));
  ASSERT_TRUE(r.position(2.5) == Point3f(4.5, 3, 4));
}
