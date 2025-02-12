#include "geometry/cone.h"
#include "geometry/cube.h"
#include "geometry/cylinder.h"
#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "geometry/triangle.h"
#include "gtesting.h"
#include "transformations/transformation.h"

class TCone : public testing::RTest {
 public:
  Cone cone;
  Ray r;
  Intersection rec;
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
  Intersection rec;
};

class PlaneTest : public testing::RTest {
 public:
  Plane p;
  Ray r;
  Intersection rec;
};

class SphereTest : public testing::RTest {
 public:
  Sphere s;
  Ray r;
  Intersection rec;
};

class TriangleTest : public testing::RTest {
 public:
  Ray r;
  Point3D p1 = Point3D(0.f, 1.f, 0.f);
  Point3D p2 = Point3D(-1.f, 0.f, 0.f);
  Point3D p3 = Point3D(1.f, 0.f, 0.f);
  Triangle t = Triangle({p1, p2, p3});
  Intersection rec;
};

class SmoothTriangleTest : public testing::RTest {
 public:
  Point3D p1 = Point3D(0.f, 1.f, 0.f);
  Point3D p2 = Point3D(-1.f, 0.f, 0.f);
  Point3D p3 = Point3D(1.f, 0.f, 0.f);
  Normal3D v1 = Normal3D(0.f, 1.f, 0.f);
  Normal3D v2 = Normal3D(-1.f, 0.f, 0.f);
  Normal3D v3 = Normal3D(1.f, 0.f, 0.f);
  SmoothTriangle t = SmoothTriangle(p1, p2, p3, v1, v2, v3);
};

/*==================================================================
 *		Cone Tests
 *=================================================================*/

/*TEST_F(TCone, intersectConeWithRay) {
  cone = Cone();
  r = Ray(Point3D(0.f, 0.f, -5.f), Vec3D(0.f, 0.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cone.intersect(r, rec));
  EXPECT_FLOAT_EQ(rec.t1, 5.f);
  EXPECT_FLOAT_EQ(rec.t2, 5.f);
  r = Ray(Point3D(0.f, 0.f, -5.f), Vec3D(1.f, 1.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cone.intersect(r, rec));
  EXPECT_FLOAT_EQ(rec.t1, 8.66025f);
  EXPECT_FLOAT_EQ(rec.t2, 8.66025f);
  r = Ray(Point3D(1.f, 1.f, -5.f), Vec3D(-0.5f, -1.f, 1.f).normalize());
  rec = IntersectionRecord();
  EXPECT_TRUE(cone.intersect(r, rec));
  float eps = 1E-4f;
  EXPECT_NEAR(rec.t1, 4.55006f, eps);
  EXPECT_NEAR(rec.t2, 49.44994f, eps);
}

TEST_F(TCone, intersectConeWithRayParallelToOneHalf) {
cone = Cone();
r = Ray(Point3D(0.f, 0.f, -1.f), Vec3D(0.f, 1.f, 1.f).normalize());
rec = IntersectionRecord();
EXPECT_TRUE(cone.intersect(r, rec));
EXPECT_NEAR(rec.t1, 0.35355f, 1E-5f);
}

TEST_F(TCone, intersectConesEndCaps) {
cone = Cone(0.5f, 0.5f, true);
r = Ray(Point3D(0.f, 0.f, -5.f), Vec3D(0.f, 0.f, 1.f).normalize());
rec = IntersectionRecord();
EXPECT_TRUE(cone.intersect(r, rec));
EXPECT_FLOAT_EQ(rec.t1, 5.f);
EXPECT_FLOAT_EQ(rec.t2, 5.f);
r = Ray(Point3D(0.f, 0.f, -5.f), Vec3D(1.f, 1.f, 1.f).normalize());
rec = IntersectionRecord();
EXPECT_TRUE(cone.intersect(r, rec));
EXPECT_FLOAT_EQ(rec.t1, 8.66025f);
EXPECT_FLOAT_EQ(rec.t2, 8.66025f);
r = Ray(Point3D(1.f, 1.f, -5.f), Vec3D(-0.5f, -1.f, 1.f).normalize());
rec = IntersectionRecord();
EXPECT_TRUE(cone.intersect(r, rec));
float eps = 1E-4f;
EXPECT_NEAR(rec.t1, 4.55006f, eps);
EXPECT_NEAR(rec.t2, 49.44994f, eps);
}*/

/*==================================================================
 *		Cube Tests
 *=================================================================*/

TEST_F(CubeTest, rayIntersectsCube) {
  box = std::make_shared<Cube>();

  ray = Ray(Point3D(5.f, 0.5f, 0.f), Vec3D(-1.f, 0.f, 0.f));
  auto rec = Intersection{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_EQ(rec.min_hit, 4.f);

  ray = Ray(Point3D(-5.f, 0.5f, 0.f), Vec3D(1.f, 0.f, 0.f));
  rec = Intersection{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_EQ(rec.min_hit, 4.f);

  ray = Ray(Point3D(0.5f, 5.f, 0.f), Vec3D(0.f, -1.f, 0.f));
  rec = Intersection{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_EQ(rec.min_hit, 4.f);

  ray = Ray(Point3D(0.5f, -5.f, 0.f), Vec3D(0.f, 1.f, 0.f));
  rec = Intersection{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_EQ(rec.min_hit, 4.f);

  ray = Ray(Point3D(0.5f, 0.f, 5.f), Vec3D(0.f, 0.f, -1.f));
  rec = Intersection{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_EQ(rec.min_hit, 4.f);

  ray = Ray(Point3D(0.5f, 0.f, -5.f), Vec3D(0.f, 0.f, 1.f));
  rec = Intersection{};
  EXPECT_TRUE(box->intersect(ray, rec));
  EXPECT_EQ(rec.min_hit, 4.f);
}

TEST_F(CubeTest, rayMissesCube) {
  box = std::make_shared<Cube>();

  ray = Ray(Point3D(-2.f, 0.f, 0.f), Vec3D(0.2673f, 0.5345f, 0.8018f));
  auto rec = Intersection{};
  EXPECT_FALSE(box->intersect(ray, rec));

  ray = Ray(Point3D(0.f, -2.f, 0.f), Vec3D(0.8018f, 0.2673f, 0.5345f));
  rec = Intersection{};
  EXPECT_FALSE(box->intersect(ray, rec));

  ray = Ray(Point3D(0.f, 0.f, -2.f), Vec3D(0.5345f, 0.8018f, 0.2673f));
  rec = Intersection{};
  EXPECT_FALSE(box->intersect(ray, rec));

  ray = Ray(Point3D(2.f, 0.f, 2.f), Vec3D(0.f, 0.f, -1.f));
  rec = Intersection{};
  EXPECT_FALSE(box->intersect(ray, rec));

  ray = Ray(Point3D(0.f, 2.f, 2.f), Vec3D(0.f, -1.f, 0.f));
  rec = Intersection{};
  EXPECT_FALSE(box->intersect(ray, rec));

  ray = Ray(Point3D(2.f, 2.f, 0.f), Vec3D(-1.f, 0.f, 0.f));
  rec = Intersection{};
  EXPECT_FALSE(box->intersect(ray, rec));
}

TEST_F(CubeTest, normalOnSurfaceOfCube) {
  box = std::make_shared<Cube>();
  auto point = Point3D(1.f, 0.5f, -0.8f);
  EXPECT_TRUE(box->normal(point) == Vec3D(1.f, 0.f, 0.f));
  point = Point3D(-1.f, -0.2f, 0.9f);
  EXPECT_TRUE(box->normal(point) == Vec3D(-1.f, 0.f, 0.f));
  point = Point3D(-0.4f, 1.f, -0.1f);
  EXPECT_TRUE(box->normal(point) == Vec3D(0.f, 1.f, 0.f));
  point = Point3D(0.3f, -1.f, -0.7f);
  EXPECT_TRUE(box->normal(point) == Vec3D(0.f, -1.f, 0.f));
  point = Point3D(-0.6f, 0.3f, 1.f);
  EXPECT_TRUE(box->normal(point) == Vec3D(0.f, 0.f, 1.f));
  point = Point3D(0.4f, 0.4f, -1.f);
  EXPECT_TRUE(box->normal(point) == Vec3D(0.f, 0.f, -1.f));
  point = Point3D(1.f, 1.f, 1.f);
  EXPECT_TRUE(box->normal(point) == Vec3D(1.f, 0.f, 0.f));
  point = Point3D(-1.f, -1.f, -1.f);
  EXPECT_TRUE(box->normal(point) == Vec3D(-1.f, 0.f, 0.f));
}

/*==================================================================
 *		Cylinder Tests
 *=================================================================*/

TEST_F(CylinderTest, rayMissesTheCylinder) {
  cyl = Cylinder();
  r = Ray(Point3D(1.f, 0.f, 0.f), getUnitVectorOf(Vec3D(0.f, 1.f, 0.f)));
  rec = Intersection();
  EXPECT_FALSE(cyl.intersect(r, rec));
  r = Ray(Point3D(0.f, 0.f, 0.f), getUnitVectorOf(Vec3D(0.f, 1.f, 0.f)));
  rec = Intersection();
  EXPECT_FALSE(cyl.intersect(r, rec));
  r = Ray(Point3D(0.f, 0.f, -5.f), getUnitVectorOf(Vec3D(1.f, 1.f, 1.f)));
  rec = Intersection();
  EXPECT_FALSE(cyl.intersect(r, rec));
}

TEST_F(CylinderTest, rayHitsTheCylinder) {
  cyl = Cylinder();
  r = Ray(Point3D(1.f, 0.f, -5.f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  rec = Intersection();
  EXPECT_TRUE(cyl.intersect(r, rec));
  EXPECT_FLOAT_EQ(rec.min_hit, 5.f);
  r = Ray(Point3D(0.f, 0.f, -5.f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  rec = Intersection();
  EXPECT_TRUE(cyl.intersect(r, rec));
  EXPECT_FLOAT_EQ(rec.min_hit, 4.f);
  r = Ray(Point3D(0.5f, 0.f, -5.f), getUnitVectorOf(Vec3D(0.1f, 1.f, 1.f)));
  rec = Intersection();
  EXPECT_TRUE(cyl.intersect(r, rec));
  float eps = 10E-5f;
  EXPECT_NEAR(rec.min_hit, 6.80798f, eps);
}

TEST_F(CylinderTest, normalVectorOnCylinder) {
  cyl = Cylinder();
  Point3D point(1.f, 0.f, 0.f);

  auto vec = cyl.normal(point);
  compareVectors(vec, Vec3D(1.f, 0.f, 0.f));

  point = Point3D(0.f, 5.f, -1.f);
  vec = cyl.normal(point);
  compareVectors(vec, Vec3D(0.f, 0.f, -1.f));

  point = Point3D(0.f, -2.f, 1.f);
  vec = cyl.normal(point);
  compareVectors(vec, Vec3D(0.f, 0.f, 1.f));

  point = Point3D(-1.f, 1.f, 0.f);
  vec = cyl.normal(point);
  compareVectors(vec, Vec3D(-1.f, 0.f, 0.f));
}

TEST_F(CylinderTest, intersectConstrainedCylinder) {
  cyl = Cylinder(1, 2);
  r = Ray(Point3D(0.f, 1.5f, 0.f), getUnitVectorOf(Vec3D(0.1f, 1.f, 0.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
  r = Ray(Point3D(0.f, 3.f, -5.f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
  r = Ray(Point3D(0.f, 0.f, -5.f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
  r = Ray(Point3D(0.f, 2.f, -5.f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
  r = Ray(Point3D(0.f, 1.f, -5.f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
  r = Ray(Point3D(0.f, 1.5f, -2.f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
}

TEST_F(CylinderTest, intersectClosedCylinder) {
  cyl = Cylinder(1, 2, true);
  r = Ray(Point3D(0.f, 3.f, 0.f), getUnitVectorOf(Vec3D(0.1f, -1.f, 0.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
  r = Ray(Point3D(0.f, 3.f, -2.f), getUnitVectorOf(Vec3D(0.f, -1.f, 2.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
  r = Ray(Point3D(0.f, 4.f, -2.f), getUnitVectorOf(Vec3D(0.f, -1.f, 1.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
  r = Ray(Point3D(0.f, 0.f, -2.f), getUnitVectorOf(Vec3D(0.f, 1.f, 2.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
  r = Ray(Point3D(0.f, -1.f, -2.f), getUnitVectorOf(Vec3D(0.f, 1.f, 1.f)));
  rec = Intersection();
  cyl.intersect(r, rec);
}

TEST_F(CylinderTest, normalVectorOnEndCaps) {
  cyl = Cylinder(1, 2, true);
  r = Ray(Point3D(0.f, 3.f, 0.f), getUnitVectorOf(Vec3D(0.1f, -1.f, 0.f)));

  Vec3D vect1(0.f, -1.f, 0.f);
  Vec3D vect2(0.f, 1.f, 0.f);

  Point3D point(0.f, 1.f, 0.f);
  auto vec = cyl.normal(point);
  compareVectors(vec, vect1);

  point = Point3D(0.5f, 1.f, 0.f);
  vec = cyl.normal(point);
  compareVectors(vec, vect1);

  point = Point3D(0.f, 1.f, 0.5f);
  vec = cyl.normal(point);
  compareVectors(vec, vect1);

  point = Point3D(0.f, 2.f, 0.f);
  vec = cyl.normal(point);
  compareVectors(vec, vect2);

  point = Point3D(0.5f, 2.f, 0.f);
  vec = cyl.normal(point);
  compareVectors(vec, vect2);

  point = Point3D(0.f, 2.f, 0.5f);
  vec = cyl.normal(point);
  compareVectors(vec, vect2);
}

/*==================================================================
 *		Plane Tests
 *=================================================================*/

TEST_F(PlaneTest, normalOfPlaneIsConstantEverywhere) {
  p = Plane();
  auto n1 = p.normal(Point3D(0.0f, 0.0f, 0.0f));
  auto n2 = p.normal(Point3D(10.0f, 0.0f, -10.0f));
  auto n3 = p.normal(Point3D(-5.0f, 0.0f, 150.0f));

  ASSERT_TRUE(n1 == Vec3D(0.0f, 1.0f, 0.0f));
  ASSERT_TRUE(n2 == Vec3D(0.0f, 1.0f, 0.0f));
  ASSERT_TRUE(n3 == Vec3D(0.0f, 1.0f, 0.0f));
}

TEST_F(PlaneTest, intersectRayParallelToThePlane) {
  p = Plane();
  r = Ray(Point3D(0.0f, 10.0f, 0.0f), Vec3D(0.0f, 0.0f, 1.0f));
  ASSERT_FALSE(p.intersect(r, rec));
}

TEST_F(PlaneTest, intersectWithACoplanarRay) {
  p = Plane();
  r = Ray(Point3D(0.0f, 0.0f, 0.0f), Vec3D(0.0f, 0.0f, 1.0f));
  ASSERT_FALSE(p.intersect(r, rec));
}

TEST_F(PlaneTest, rayIntersectingPlaneFromAbove) {
  p = Plane();
  r = Ray(Point3D(0.0f, 1.0f, 0.0f), Vec3D(0.0f, -1.0f, 0.0f));
  ASSERT_TRUE(p.intersect(r, rec));
  ASSERT_TRUE(rec.min_hit == 1.0f);
}

TEST_F(PlaneTest, rayIntersectingPlaneFromBelow) {
  p = Plane();
  r = Ray(Point3D(0.0f, -1.0f, 0.0f), Vec3D(0.0f, 1.0f, 0.0f));
  ASSERT_TRUE(p.intersect(r, rec));
  ASSERT_TRUE(rec.min_hit == 1.0f);
}

/*==================================================================
 *		Sphere Tests
 *=================================================================*/

TEST_F(SphereTest, raySphereIntersection) {
  s = Sphere(Point3D(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r, rec));
  ASSERT_EQ(rec.min_hit, 4.0f);
}

TEST_F(SphereTest, raySphereTangentIntersection) {
  s = Sphere(Point3D(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3D(0.0f, 1.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r, rec));
  ASSERT_EQ(rec.min_hit, 5.0f);
}

TEST_F(SphereTest, raySphereNoIntersection) {
  s = Sphere(Point3D(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3D(0.0f, 2.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  ASSERT_FALSE(s.intersect(r, rec));
  ASSERT_EQ(rec.min_hit, std::numeric_limits<float>::infinity());
}

TEST_F(SphereTest, rayOriginAtSphereCenterIntersection) {
  s = Sphere(Point3D(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3D(0.0f, 0.0f, 0.0f), Vec3D(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r, rec));
  ASSERT_EQ(rec.min_hit, 1.0f);
}

TEST_F(SphereTest, SphereIsBehindOrigin) {
  /*s = Sphere(Point3D(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3D(0.0f, 0.0f, 5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r, rec));
  ASSERT_EQ(rec.t1, -6.0f);
  ASSERT_EQ(rec.t2, -4.0f);*/
}

TEST_F(SphereTest, testingTheNextTest) {
  s = Sphere(Point3D(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  Transformation trans(scale(2.0f, 2.0f, 2.0f));
  Ray r1 = trans.worldToObjectSpace(r);
  ASSERT_TRUE(s.intersect(r1, rec));
  ASSERT_EQ(rec.min_hit, 3.0f);
}

TEST_F(SphereTest, transformsSphere) {
  r = Ray(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  SceneElement* t = new Sphere(Point3D(0.0f, 0.0f, 0.0f), 1.0f);
  auto transformation = Transformation(scale(2.0f, 2.0f, 2.0f));
  t->setTransformation(transformation);
  ASSERT_TRUE(t->intersect(r, rec));
  ASSERT_EQ(rec.min_hit, 3.0f);
  delete t;
}

TEST_F(SphereTest, translatesSphere) {
  r = Ray(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  SceneElement* t = new Sphere(Point3D(0.0f, 0.0f, 0.0f), 1.0f);
  auto transformation = Transformation(translation(5.0f, 0.0f, 0.0f));
  t->setTransformation(transformation);
  ASSERT_FALSE(t->intersect(r, rec));
  delete t;
}

TEST_F(SphereTest, returnsSurfaceNormalOnX) {
  s = Sphere();
  auto n = s.normal(Point3D(1, 0, 0));
  ASSERT_TRUE(n == Vec3D(1, 0, 0));
}

TEST_F(SphereTest, returnsSurfaceNormalOnY) {
  s = Sphere();
  auto n = s.normal(Point3D(0, 1, 0));
  ASSERT_TRUE(n == Vec3D(0, 1, 0));
}

TEST_F(SphereTest, returnsSurfaceNormalOnZ) {
  s = Sphere();
  auto n = s.normal(Point3D(0, 0, 1));
  ASSERT_TRUE(n == Vec3D(0, 0, 1));
}

TEST_F(SphereTest, returnsNonAxialSurfaceNormal) {
  s = Sphere();
  auto n = s.normal(
      Point3D(sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f));
  Vec3D n1 = Vec3D(sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f);
  compareVectors(n, n1);
}

TEST_F(SphereTest, normalIsNormalizedVector) {
  s = Sphere();
  auto n = s.normal(
      Point3D(sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f, sqrt(3.0f) / 3.0f));
  EXPECT_NEAR(1.f, getUnitVectorOf(n).length(), 1.E-6);
}

/*==================================================================
 *		Triangle Tests
 *=================================================================*/

TEST_F(TriangleTest, constructingTriangle) {
  comparePoints(t.point(0), p1);
  comparePoints(t.point(1), p2);
  comparePoints(t.point(2), p3);
  compareVectors(t.normal(Point3D()), Vec3D(0.f, 0.f, 1.f));
  compareVectors(t.edgeVector(0), Vec3D(-1.f, -1.f, 0.f));
  compareVectors(t.edgeVector(1), Vec3D(1.f, -1.f, 0.f));
}

TEST_F(TriangleTest, normalVectorOfTriangle) {
  auto v1 = t.normal(Point3D(0.f, 0.5f, 0.f));
  auto v2 = t.normal(Point3D(1.f, 1.5f, 1.f));
  auto v3 = t.normal(Point3D(0.453f, 0.5f, 6.f));
  ASSERT_TRUE(v1 == v2);
  ASSERT_TRUE(v1 == v3);
}

TEST_F(TriangleTest, intersectingRayParallelToTriangle) {
  Ray r(Point3D(0.f, -1.f, -2.f), Vec3D(0.f, 1.f, 0.f));
  ASSERT_FALSE(t.intersect(r, rec));
}

TEST_F(TriangleTest, rayMissesP1P3Edge) {
  Ray r(Point3D(1.f, 1.f, -2.f), Vec3D(0.f, 0.f, 1.f));
  ASSERT_FALSE(t.intersect(r, rec));
}

TEST_F(TriangleTest, rayMissesP1P2Edge) {
  Ray r(Point3D(-1.f, 1.f, -2.f), Vec3D(0.f, 0.f, 1.f));
  ASSERT_FALSE(t.intersect(r, rec));
}

TEST_F(TriangleTest, rayMissesP2P3Edge) {
  Ray r(Point3D(0.f, -1.f, -2.f), Vec3D(0.f, 0.f, 1.f));
  ASSERT_FALSE(t.intersect(r, rec));
}

TEST_F(TriangleTest, rayStrikesTriangle) {
  Ray r(Point3D(0.f, 0.5f, -2.f), Vec3D(0.f, 0.f, 1.f));
  ASSERT_TRUE(t.intersect(r, rec));
  ASSERT_TRUE(rec.min_hit == 2);
}

TEST_F(SmoothTriangleTest, propertiesAreCorrect) {
  comparePoints(t.points(0), Point3D(0.f, 1.f, 0.f));
  comparePoints(t.points(1), Point3D(-1.f, 0.f, 0.f));
  comparePoints(t.points(2), Point3D(1.f, 0.f, 0.f));
  compareVectors(t.normals(0), Vec3D(0.f, 1.f, 0.f));
  compareVectors(t.normals(1), Vec3D(-1.f, 0.f, 0.f));
  compareVectors(t.normals(2), Vec3D(1.f, 0.f, 0.f));
}

TEST_F(SmoothTriangleTest, normalInterpolation) {
  auto r = Ray(Point3D(-0.2f, 0.3f, -2.f), Vec3D(0.f, 0.f, 1.f));
  Intersection rec;
  t.intersect(r, rec);
  auto v = t.normal(Point3D());
  compareVectorsApprox(v, Vec3D(-0.2f, 0.3f, 0.f), EPS);
}
