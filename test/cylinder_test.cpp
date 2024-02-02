#include "geometry/cylinder.h"

#include "gtest/gtest.h"
#include "tools/ray.h"

using namespace testing;

class CylinderTest : public Test {
 public:
  Cylinder cyl;
  Ray r;
  IntersectionRecord rec;
};

TEST_F(CylinderTest, rayMissesTheCylinder) {
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

TEST_F(CylinderTest, rayHitsTheCylinder) {
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

TEST_F(CylinderTest, normalVectorOnCylinder) {
  cyl = Cylinder();
  Point3f point(1.f, 0.f, 0.f);

  Vec3f vec = cyl.normal(point);
  EXPECT_FLOAT_EQ(vec.x(), 1.f);
  EXPECT_FLOAT_EQ(vec.y(), 0.f);
  EXPECT_FLOAT_EQ(vec.z(), 0.f);

  point = Point3f(0.f, 5.f, -1.f);
  vec = cyl.normal(point);
  EXPECT_FLOAT_EQ(vec.x(), 0.f);
  EXPECT_FLOAT_EQ(vec.y(), 0.f);
  EXPECT_FLOAT_EQ(vec.z(), -1.f);

  point = Point3f(0.f, -2.f, 1.f);
  vec = cyl.normal(point);
  EXPECT_FLOAT_EQ(vec.x(), 0.f);
  EXPECT_FLOAT_EQ(vec.y(), 0.f);
  EXPECT_FLOAT_EQ(vec.z(), 1.f);

  point = Point3f(-1.f, 1.f, 0.f);
  vec = cyl.normal(point);
  EXPECT_FLOAT_EQ(vec.x(), -1.f);
  EXPECT_FLOAT_EQ(vec.y(), 0.f);
  EXPECT_FLOAT_EQ(vec.z(), 0.f);
}

TEST_F(CylinderTest, intersectConstrainedCylinder) {
  cyl = Cylinder(1, 2);
  r = Ray(Point3f(0.f, 1.5f, 0.f), Vec3f(0.1f, 1.f, 0.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 3.f, -5.f), Vec3f(0.f, 0.f, 1.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 2.f, -5.f), Vec3f(0.f, 0.f, 1.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 1.f, -5.f), Vec3f(0.f, 0.f, 1.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 1.5f, -2.f), Vec3f(0.f, 0.f, 1.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 2);
}

TEST_F(CylinderTest, intersectClosedCylinder) {
  cyl = Cylinder(1, 2, true);
  r = Ray(Point3f(0.f, 3.f, 0.f), Vec3f(0.1f, -1.f, 0.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_EQ(rec.count, 2);
  r = Ray(Point3f(0.f, 3.f, -2.f), Vec3f(0.f, -1.f, 2.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_EQ(rec.count, 2);
  r = Ray(Point3f(0.f, 4.f, -2.f), Vec3f(0.f, -1.f, 1.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_EQ(rec.count, 1);
  r = Ray(Point3f(0.f, 0.f, -2.f), Vec3f(0.f, 1.f, 2.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_EQ(rec.count, 2);
  r = Ray(Point3f(0.f, -1.f, -2.f), Vec3f(0.f, 1.f, 1.f).normalize());
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_EQ(rec.count, 1);
}

TEST_F(CylinderTest, normalVectorOnEndCaps) {
  cyl = Cylinder(1, 2, true);
  r = Ray(Point3f(0.f, 3.f, 0.f), Vec3f(0.1f, -1.f, 0.f).normalize());
  Point3f point(0.f, 1.f, 0.f);
  Vec3f vec = cyl.normal(point);
  EXPECT_FLOAT_EQ(vec.x(), 0.f);
  EXPECT_FLOAT_EQ(vec.y(), -1.f);
  EXPECT_FLOAT_EQ(vec.z(), 0.f);
  point = Point3f(0.5f, 1.f, 0.f);
  vec = cyl.normal(point);
  EXPECT_FLOAT_EQ(vec.x(), 0.f);
  EXPECT_FLOAT_EQ(vec.y(), -1.f);
  EXPECT_FLOAT_EQ(vec.z(), 0.f);
  point = Point3f(0.f, 1.f, 0.5f);
  vec = cyl.normal(point);
  EXPECT_FLOAT_EQ(vec.x(), 0.f);
  EXPECT_FLOAT_EQ(vec.y(), -1.f);
  EXPECT_FLOAT_EQ(vec.z(), 0.f);
  point = Point3f(0.f, 2.f, 0.f);
  vec = cyl.normal(point);
  EXPECT_FLOAT_EQ(vec.x(), 0.f);
  EXPECT_FLOAT_EQ(vec.y(), 1.f);
  EXPECT_FLOAT_EQ(vec.z(), 0.f);
  point = Point3f(0.5f, 2.f, 0.f);
  vec = cyl.normal(point);
  EXPECT_FLOAT_EQ(vec.x(), 0.f);
  EXPECT_FLOAT_EQ(vec.y(), 1.f);
  EXPECT_FLOAT_EQ(vec.z(), 0.f);
  point = Point3f(0.f, 2.f, 0.5f);
  vec = cyl.normal(point);
  EXPECT_FLOAT_EQ(vec.x(), 0.f);
  EXPECT_FLOAT_EQ(vec.y(), 1.f);
  EXPECT_FLOAT_EQ(vec.z(), 0.f);
}
