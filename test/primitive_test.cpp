#include "geometry/cone.h"
#include "geometry/cube.h"
#include "geometry/cylinder.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "geometry/triangle.h"
#include "gtesting.h"
#include "transformations/transformer.h"

class TCone : public testing::RTest {
 public:
  Cone cone;
  Ray r;
  IntersectionRecord rec;
};

class CubeTest : public testing::RTest {
 public:
  SceneElementPtr box;
  Ray ray;
};

class CylinderTest : public testing::RTest {
 public:
  Cylinder cyl;
  Ray r;
  IntersectionRecord rec;
};

class PlaneTest : public testing::RTest {
 public:
  Plane p;
  Ray r;
  IntersectionRecord rec;
};

class SphereTest : public testing::RTest {
 public:
  Sphere s;
  Ray r;
  IntersectionRecord rec;
};

class TriangleTest : public testing::RTest {
 public:
  Ray r;
  Point3f p1 = Point3f(0.f, 1.f, 0.f);
  Point3f p2 = Point3f(-1.f, 0.f, 0.f);
  Point3f p3 = Point3f(1.f, 0.f, 0.f);
  Triangle t = Triangle({p1, p2, p3});
  IntersectionRecord rec;
};

class SmoothTriangleTest : public testing::RTest {
 public:
  Point3f p1 = Point3f(0.f, 1.f, 0.f);
  Point3f p2 = Point3f(-1.f, 0.f, 0.f);
  Point3f p3 = Point3f(1.f, 0.f, 0.f);
  Vec3f v1 = Vec3f(0.f, 1.f, 0.f);
  Vec3f v2 = Vec3f(-1.f, 0.f, 0.f);
  Vec3f v3 = Vec3f(1.f, 0.f, 0.f);
  SmoothTriangle t = SmoothTriangle(p1, p2, p3, v1, v2, v3);
};

/*==================================================================
 *		Cone Tests
 *=================================================================*/

/*TEST_F(TCone, intersectConeWithRay) {
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
}*/

/*==================================================================
 *		Cube Tests
 *=================================================================*/

TEST_F(CubeTest, rayIntersectsCube) {
  box = std::make_shared<Cube>();

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

TEST_F(CubeTest, rayMissesCube) {
  box = std::make_shared<Cube>();

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

TEST_F(CubeTest, normalOnSurfaceOfCube) {
  box = std::make_shared<Cube>();
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

/*==================================================================
 *		Cylinder Tests
 *=================================================================*/

TEST_F(CylinderTest, rayMissesTheCylinder) {
  cyl = Cylinder();
  r = Ray(Point3f(1.f, 0.f, 0.f), getUnitVectorOf(Vec3f(0.f, 1.f, 0.f)));
  rec = IntersectionRecord();
  EXPECT_FALSE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 0.f, 0.f), getUnitVectorOf(Vec3f(0.f, 1.f, 0.f)));
  rec = IntersectionRecord();
  EXPECT_FALSE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 0.f, -5.f), getUnitVectorOf(Vec3f(1.f, 1.f, 1.f)));
  rec = IntersectionRecord();
  EXPECT_FALSE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 0);
}

TEST_F(CylinderTest, rayHitsTheCylinder) {
  cyl = Cylinder();
  r = Ray(Point3f(1.f, 0.f, -5.f), getUnitVectorOf(Vec3f(0.f, 0.f, 1.f)));
  rec = IntersectionRecord();
  EXPECT_TRUE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_FLOAT_EQ(rec.t1, 5.f);
  EXPECT_FLOAT_EQ(rec.t2, 5.f);
  r = Ray(Point3f(0.f, 0.f, -5.f), getUnitVectorOf(Vec3f(0.f, 0.f, 1.f)));
  rec = IntersectionRecord();
  EXPECT_TRUE(cyl.intersect(r, rec));
  EXPECT_TRUE(rec.count == 2);
  EXPECT_FLOAT_EQ(rec.t1, 4.f);
  EXPECT_FLOAT_EQ(rec.t2, 6.f);
  r = Ray(Point3f(0.5f, 0.f, -5.f), getUnitVectorOf(Vec3f(0.1f, 1.f, 1.f)));
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
  compareVectors(vec, Vec3f(1.f, 0.f, 0.f));

  point = Point3f(0.f, 5.f, -1.f);
  vec = cyl.normal(point);
  compareVectors(vec, Vec3f(0.f, 0.f, -1.f));

  point = Point3f(0.f, -2.f, 1.f);
  vec = cyl.normal(point);
  compareVectors(vec, Vec3f(0.f, 0.f, 1.f));

  point = Point3f(-1.f, 1.f, 0.f);
  vec = cyl.normal(point);
  compareVectors(vec, Vec3f(-1.f, 0.f, 0.f));
}

TEST_F(CylinderTest, intersectConstrainedCylinder) {
  cyl = Cylinder(1, 2);
  r = Ray(Point3f(0.f, 1.5f, 0.f), getUnitVectorOf(Vec3f(0.1f, 1.f, 0.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 3.f, -5.f), getUnitVectorOf(Vec3f(0.f, 0.f, 1.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 0.f, -5.f), getUnitVectorOf(Vec3f(0.f, 0.f, 1.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 2.f, -5.f), getUnitVectorOf(Vec3f(0.f, 0.f, 1.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 1.f, -5.f), getUnitVectorOf(Vec3f(0.f, 0.f, 1.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 0);
  r = Ray(Point3f(0.f, 1.5f, -2.f), getUnitVectorOf(Vec3f(0.f, 0.f, 1.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_TRUE(rec.count == 2);
}

TEST_F(CylinderTest, intersectClosedCylinder) {
  cyl = Cylinder(1, 2, true);
  r = Ray(Point3f(0.f, 3.f, 0.f), getUnitVectorOf(Vec3f(0.1f, -1.f, 0.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_EQ(rec.count, 2);
  r = Ray(Point3f(0.f, 3.f, -2.f), getUnitVectorOf(Vec3f(0.f, -1.f, 2.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_EQ(rec.count, 2);
  r = Ray(Point3f(0.f, 4.f, -2.f), getUnitVectorOf(Vec3f(0.f, -1.f, 1.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_EQ(rec.count, 1);
  r = Ray(Point3f(0.f, 0.f, -2.f), getUnitVectorOf(Vec3f(0.f, 1.f, 2.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_EQ(rec.count, 2);
  r = Ray(Point3f(0.f, -1.f, -2.f), getUnitVectorOf(Vec3f(0.f, 1.f, 1.f)));
  rec = IntersectionRecord();
  cyl.intersect(r, rec);
  EXPECT_EQ(rec.count, 1);
}

TEST_F(CylinderTest, normalVectorOnEndCaps) {
  cyl = Cylinder(1, 2, true);
  r = Ray(Point3f(0.f, 3.f, 0.f), getUnitVectorOf(Vec3f(0.1f, -1.f, 0.f)));

  Vec3f vect1(0.f, -1.f, 0.f);
  Vec3f vect2(0.f, 1.f, 0.f);

  Point3f point(0.f, 1.f, 0.f);
  Vec3f vec = cyl.normal(point);
  compareVectors(vec, vect1);

  point = Point3f(0.5f, 1.f, 0.f);
  vec = cyl.normal(point);
  compareVectors(vec, vect1);

  point = Point3f(0.f, 1.f, 0.5f);
  vec = cyl.normal(point);
  compareVectors(vec, vect1);

  point = Point3f(0.f, 2.f, 0.f);
  vec = cyl.normal(point);
  compareVectors(vec, vect2);

  point = Point3f(0.5f, 2.f, 0.f);
  vec = cyl.normal(point);
  compareVectors(vec, vect2);

  point = Point3f(0.f, 2.f, 0.5f);
  vec = cyl.normal(point);
  compareVectors(vec, vect2);
}

/*==================================================================
 *		Plane Tests
 *=================================================================*/

TEST_F(PlaneTest, normalOfPlaneIsConstantEverywhere) {
  p = Plane();
  Vec3f n1 = p.normal(Point3f(0.0f, 0.0f, 0.0f));
  Vec3f n2 = p.normal(Point3f(10.0f, 0.0f, -10.0f));
  Vec3f n3 = p.normal(Point3f(-5.0f, 0.0f, 150.0f));

  ASSERT_TRUE(n1 == Vec3f(0.0f, 1.0f, 0.0f));
  ASSERT_TRUE(n2 == Vec3f(0.0f, 1.0f, 0.0f));
  ASSERT_TRUE(n3 == Vec3f(0.0f, 1.0f, 0.0f));
}

TEST_F(PlaneTest, intersectRayParallelToThePlane) {
  p = Plane();
  r = Ray(Point3f(0.0f, 10.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_FALSE(p.intersect(r, rec));
  ASSERT_TRUE(rec.count == 0);
}

TEST_F(PlaneTest, intersectWithACoplanarRay) {
  p = Plane();
  r = Ray(Point3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_FALSE(p.intersect(r, rec));
  ASSERT_TRUE(rec.count == 0);
}

TEST_F(PlaneTest, rayIntersectingPlaneFromAbove) {
  p = Plane();
  r = Ray(Point3f(0.0f, 1.0f, 0.0f), Vec3f(0.0f, -1.0f, 0.0f));
  ASSERT_TRUE(p.intersect(r, rec));
  ASSERT_TRUE(rec.count == 1);
  ASSERT_TRUE(rec.t_min() == 1.0f);
}

TEST_F(PlaneTest, rayIntersectingPlaneFromBelow) {
  p = Plane();
  r = Ray(Point3f(0.0f, -1.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
  ASSERT_TRUE(p.intersect(r, rec));
  ASSERT_TRUE(rec.count == 1);
  ASSERT_TRUE(rec.t_min() == 1.0f);
}

/*==================================================================
 *		Sphere Tests
 *=================================================================*/

TEST_F(SphereTest, raySphereIntersection) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r, rec));
  ASSERT_EQ(rec.count, 2);
  ASSERT_EQ(rec.t1, 4.0f);
  ASSERT_EQ(rec.t2, 6.0f);
}

TEST_F(SphereTest, raySphereTangentIntersection) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 1.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r, rec));
  ASSERT_EQ(rec.count, 2);
  ASSERT_EQ(rec.t1, 5.0f);
  ASSERT_EQ(rec.t2, 5.0f);
}

TEST_F(SphereTest, raySphereNoIntersection) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 2.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_FALSE(s.intersect(r, rec));
  ASSERT_EQ(rec.count, 0);
  ASSERT_EQ(rec.t1, -MAXFLOAT);
  ASSERT_EQ(rec.t2, -MAXFLOAT);
}

TEST_F(SphereTest, rayOriginAtSphereCenterIntersection) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r, rec));
  ASSERT_EQ(rec.count, 2);
  ASSERT_EQ(rec.t1, -1.0f);
  ASSERT_EQ(rec.t2, 1.0f);
}

TEST_F(SphereTest, SphereIsBehindOrigin) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 0.0f, 5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r, rec));
  ASSERT_EQ(rec.count, 2);
  ASSERT_EQ(rec.t1, -6.0f);
  ASSERT_EQ(rec.t2, -4.0f);
}

TEST_F(SphereTest, testingTheNextTest) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  Ray r1 = r.transform(scale(2.0f, 2.0f, 2.0f).inverse());
  ASSERT_TRUE(s.intersect(r1, rec));
  ASSERT_EQ(rec.count, 2);
  ASSERT_EQ(rec.t1, 3.0f);
  ASSERT_EQ(rec.t2, 7.0f);
}

TEST_F(SphereTest, transformsSphere) {
  r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  SceneElement* t = new Transformer(new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f),
                                    scale(2.0f, 2.0f, 2.0f));
  ASSERT_TRUE(t->intersect(r, rec));
  ASSERT_EQ(rec.count, 2);
  ASSERT_EQ(rec.t1, 3.0f);
  ASSERT_EQ(rec.t2, 7.0f);
  delete t;
}

TEST_F(SphereTest, translatesSphere) {
  r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  SceneElement* t = new Transformer(new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f),
                                    translation(5.0f, 0.0f, 0.0f));
  ASSERT_FALSE(t->intersect(r, rec));
  ASSERT_TRUE(rec.count == 0);
  delete t;
}

TEST_F(SphereTest, returnsSurfaceNormalOnX) {
  s = Sphere();
  Vec3f n = s.normal(Point3f(1, 0, 0));
  ASSERT_TRUE(n == Vec3f(1, 0, 0));
}

TEST_F(SphereTest, returnsSurfaceNormalOnY) {
  s = Sphere();
  Vec3f n = s.normal(Point3f(0, 1, 0));
  ASSERT_TRUE(n == Vec3f(0, 1, 0));
}

TEST_F(SphereTest, returnsSurfaceNormalOnZ) {
  s = Sphere();
  Vec3f n = s.normal(Point3f(0, 0, 1));
  ASSERT_TRUE(n == Vec3f(0, 0, 1));
}

TEST_F(SphereTest, returnsNonAxialSurfaceNormal) {
  s = Sphere();
  Vec3f n = s.normal(
      Point3f(sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f));
  Vec3f n1 = Vec3f(sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f);
  compareVectors(n, n1);
}

TEST_F(SphereTest, normalIsNormalizedVector) {
  s = Sphere();
  Vec3f n = s.normal(
      Point3f(sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f));
  EXPECT_NEAR(1.f, getUnitVectorOf(n).length(), 1.E-6);
}

/*==================================================================
 *		Triangle Tests
 *=================================================================*/

TEST_F(TriangleTest, constructingTriangle) {
  comparePoints(t.point(0), p1);
  comparePoints(t.point(1), p2);
  comparePoints(t.point(2), p3);
  compareVectors(t.normal(Point3f()), Vec3f(0.f, 0.f, 1.f));
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

TEST_F(SmoothTriangleTest, propertiesAreCorrect) {
  comparePoints(t.points(0), Point3f(0.f, 1.f, 0.f));
  comparePoints(t.points(1), Point3f(-1.f, 0.f, 0.f));
  comparePoints(t.points(2), Point3f(1.f, 0.f, 0.f));
  compareVectors(t.normals(0), Vec3f(0.f, 1.f, 0.f));
  compareVectors(t.normals(1), Vec3f(-1.f, 0.f, 0.f));
  compareVectors(t.normals(2), Vec3f(1.f, 0.f, 0.f));
}

TEST_F(SmoothTriangleTest, normalInterpolation) {
  auto r = Ray(Point3f(-0.2f, 0.3f, -2.f), Vec3f(0.f, 0.f, 1.f));
  IntersectionRecord rec;
  t.intersect(r, rec);
  auto v = t.normal(Point3f());
  compareVectorsApprox(v, Vec3f(-0.2f, 0.3f, 0.f), EPS);
}
