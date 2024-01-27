#include "geometry/cone.h"

#include "gtest/gtest.h"
#include "tools/ray.h"

using namespace testing;

class TCone : public Test {
 public:
  Cone cone;
  Ray r;
  IntersectionRecord rec;
};

TEST_F(TCone, intersectConeWithRay) {
  cone = Cone();
  r = Ray(Point3f(0.f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cone.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_FLOAT_EQ(rec.t1, 5.f);
  EXPECT_FLOAT_EQ(rec.t2, 5.f);
  r = Ray(Point3f(0.f, 0.f, -5.f), Vec3f(1.f, 1.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cone.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_FLOAT_EQ(rec.t1, 8.66025f);
  EXPECT_FLOAT_EQ(rec.t2, 8.66025f);
  r = Ray(Point3f(1.f, 1.f, -5.f), Vec3f(-0.5f, -1.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cone.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  float eps = 1E-4f;
  EXPECT_NEAR(rec.t1, 4.55006f, eps);
  EXPECT_NEAR(rec.t2, 49.44994f, eps);
}

TEST_F(TCone, intersectConeWithRayParallelToOneHalf) {
  cone = Cone();
  r = Ray(Point3f(0.f, 0.f, -1.f), Vec3f(0.f, 1.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cone.intersect(r, rec));
  EXPECT_TRUE(rec.count == 1);
  EXPECT_NEAR(rec.t1, 0.35355f, 1E-5f);
}

TEST_F(TCone, intersectConesEndCaps) {
  cone = Cone(0.5f, 0.5f, true);
  r = Ray(Point3f(0.f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cone.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_FLOAT_EQ(rec.t1, 5.f);
  EXPECT_FLOAT_EQ(rec.t2, 5.f);
  r = Ray(Point3f(0.f, 0.f, -5.f), Vec3f(1.f, 1.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cone.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_FLOAT_EQ(rec.t1, 8.66025f);
  EXPECT_FLOAT_EQ(rec.t2, 8.66025f);
  r = Ray(Point3f(1.f, 1.f, -5.f), Vec3f(-0.5f, -1.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cone.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  float eps = 1E-4f;
  EXPECT_NEAR(rec.t1, 4.55006f, eps);
  EXPECT_NEAR(rec.t2, 49.44994f, eps);
}
