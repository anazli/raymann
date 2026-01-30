#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "transformations/transformation.h"

using testing::Eq;

class RayTest : public testing::Test {
 public:
  Ray r;
  Point3f ori;
  Vec3f dir;
};

TEST_F(RayTest, createsRay) {
  ori = Point3f(1, 2, 3);
  dir = Vec3f(4, 5, 6);
  r = Ray(ori, dir);

  EXPECT_THAT(r.origin(), Eq(ori));
  ASSERT_THAT(r.direction(), Eq(dir));
}

TEST_F(RayTest, computesPositionAtT) {
  ori = Point3f(2, 3, 4);
  dir = Vec3f(1, 0, 0);
  r = Ray(ori, dir);

  EXPECT_THAT(r.position(0), Eq(ori));
  EXPECT_THAT(r.position(1), Eq(Point3f(3, 3, 4)));
  EXPECT_THAT(r.position(-1), Eq(Point3f(1, 3, 4)));
  ASSERT_THAT(r.position(2.5), Eq(Point3f(4.5, 3, 4)));
}

TEST_F(RayTest, translatesRay) {
  r = Ray(Point3f(1, 2, 3), Vec3f(0, 1, 0));
  Mat4D m = translation(3.0f, 4.0f, 5.0f);
  Transformation trans(m.inverse());
  Ray tr = trans.worldToObjectSpace(r);

  EXPECT_THAT(tr.origin(), Eq(Point3f(4.0f, 6.0f, 8.0f)));
  ASSERT_THAT(tr.direction(), Eq(Vec3f(0.0f, 1.0f, 0.0f)));
}

TEST_F(RayTest, scalesRay) {
  r = Ray(Point3f(1, 2, 3), Vec3f(0, 1, 0));
  Mat4D m = scale(2.0f, 3.0f, 4.0f);
  Transformation trans(m.inverse());
  Ray tr = trans.worldToObjectSpace(r);

  EXPECT_THAT(tr.origin(), Eq(Point3f(2.0f, 6.0f, 12.0f)));
  ASSERT_THAT(tr.direction(), Eq(Vec3f(0.0f, 3.0f, 0.0f)));
}
