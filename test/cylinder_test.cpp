#include "geometry/cylinder.h"

#include "gtest/gtest.h"
#include "tools/ray.h"

using namespace testing;

class TCylinder : public Test {
 public:
  Cylinder cyl;
  Ray r;
  IntersectionRecord rec;
};

TEST_F(TCylinder, rayMissesTheCylinder) {
  cyl = Cylinder();
  r = Ray(Point3f(1.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f).normalize());
  rec = IntersectionRecord();
  EXPECT_FALSE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f).normalize());
  rec = IntersectionRecord();
  EXPECT_FALSE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 0.f, -5.f), Vec3f(1.f, 1.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_FALSE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 0);
}

TEST_F(TCylinder, rayHitsTheCylinder) {
  cyl = Cylinder();
  r = Ray(Point3f(1.f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_FLOAT_EQ(rec.t1, 5.f);
  EXPECT_FLOAT_EQ(rec.t2, 5.f);
  r = Ray(Point3f(0.f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_FLOAT_EQ(rec.t1, 4.f);
  EXPECT_FLOAT_EQ(rec.t2, 6.f);
  r = Ray(Point3f(0.5f, 0.f, -5.f), Vec3f(0.1f, 1.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  float eps = 10E-5f;
  EXPECT_NEAR(rec.t1, 6.80798f, eps);
  EXPECT_NEAR(rec.t2, 7.08872f, eps);
}
