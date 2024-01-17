#include "geometry/bounding_box.h"
#include "gtest/gtest.h"
#include "tools/tools.h"

using namespace testing;
using namespace std;

class TBox : public Test {
 public:
  SceneElementPtr box;
  Ray ray;
};

TEST_F(TBox, rayIntersectsCube) {
  box = make_shared<BoundingBox>();

  ray = Ray(Point3f(5.f, 0.5f, 0.f), Vec3f(-1.f, 0.f, 0.f));
  auto rec = IntersectionRecord{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_EQ(rec.t1, 4.f);
  EXPECT_EQ(rec.t2, 6.f);

  ray = Ray(Point3f(-5.f, 0.5f, 0.f), Vec3f(1.f, 0.f, 0.f));
  rec = IntersectionRecord{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_EQ(rec.t1, 4.f);
  EXPECT_EQ(rec.t2, 6.f);

  ray = Ray(Point3f(0.5f, 5.f, 0.f), Vec3f(0.f, -1.f, 0.f));
  rec = IntersectionRecord{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_EQ(rec.t1, 4.f);
  EXPECT_EQ(rec.t2, 6.f);

  ray = Ray(Point3f(0.5f, -5.f, 0.f), Vec3f(0.f, 1.f, 0.f));
  rec = IntersectionRecord{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_EQ(rec.t1, 4.f);
  EXPECT_EQ(rec.t2, 6.f);

  ray = Ray(Point3f(0.5f, 0.f, 5.f), Vec3f(0.f, 0.f, -1.f));
  rec = IntersectionRecord{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_EQ(rec.t1, 4.f);
  EXPECT_EQ(rec.t2, 6.f);

  ray = Ray(Point3f(0.5f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f));
  rec = IntersectionRecord{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_EQ(rec.t1, 4.f);
  EXPECT_EQ(rec.t2, 6.f);
}

TEST_F(TBox, rayMissesCube) {
  box = make_shared<BoundingBox>();

  ray = Ray(Point3f(-2.f, 0.f, 0.f), Vec3f(0.2673f, 0.5345f, 0.8018f));
  auto rec = IntersectionRecord{};
  EXPECT_FALSE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 0);

  ray = Ray(Point3f(0.f, -2.f, 0.f), Vec3f(0.8018f, 0.2673f, 0.5345f));
  rec = IntersectionRecord{};
  EXPECT_FALSE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 0);

  ray = Ray(Point3f(0.f, 0.f, -2.f), Vec3f(0.5345f, 0.8018f, 0.2673f));
  rec = IntersectionRecord{};
  EXPECT_FALSE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 0);

  ray = Ray(Point3f(2.f, 0.f, 2.f), Vec3f(0.f, 0.f, -1.f));
  rec = IntersectionRecord{};
  EXPECT_FALSE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 0);

  ray = Ray(Point3f(0.f, 2.f, 2.f), Vec3f(0.f, -1.f, 0.f));
  rec = IntersectionRecord{};
  EXPECT_FALSE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 0);

  ray = Ray(Point3f(2.f, 2.f, 0.f), Vec3f(-1.f, 0.f, 0.f));
  rec = IntersectionRecord{};
  EXPECT_FALSE(box->intersect(ray, rec));
  EXPECT_TRUE(rec.count == 0);
}

TEST_F(TBox, normalOnSurfaceOfCube) {
  box = make_shared<BoundingBox>();
  auto point = Point3f(1.f, 0.5f, -0.8f);
  EXPECT_TRUE(box->normal(point) == Vec3f(1.f, 0.f, 0.f));
  point = Point3f(-1.f, -0.2f, 0.9f);
  EXPECT_TRUE(box->normal(point) == Vec3f(-1.f, 0.f, 0.f));
  point = Point3f(-0.4f, 1.f, -0.1f);
  EXPECT_TRUE(box->normal(point) == Vec3f(0.f, 1.f, 0.f));
  point = Point3f(0.3f, -1.f, -0.7f);
  EXPECT_TRUE(box->normal(point) == Vec3f(0.f, -1.f, 0.f));
  point = Point3f(-0.6f, 0.3f, 1.f);
  EXPECT_TRUE(box->normal(point) == Vec3f(0.f, 0.f, 1.f));
  point = Point3f(0.4f, 0.4f, -1.f);
  EXPECT_TRUE(box->normal(point) == Vec3f(0.f, 0.f, -1.f));
  point = Point3f(1.f, 1.f, 1.f);
  EXPECT_TRUE(box->normal(point) == Vec3f(1.f, 0.f, 0.f));
  point = Point3f(-1.f, -1.f, -1.f);
  EXPECT_TRUE(box->normal(point) == Vec3f(-1.f, 0.f, 0.f));
}