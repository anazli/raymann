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
  r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r));
  ASSERT_EQ(s.record().count, 2);
  ASSERT_EQ(s.record().t1, 4.0f);
  ASSERT_EQ(s.record().t2, 6.0f);
  ASSERT_EQ(s.name(), "Sphere:(0,0,0)");
}

TEST_F(Tsphere, raySphereTangentIntersection) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 1.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r));
  ASSERT_EQ(s.record().count, 2);
  ASSERT_EQ(s.record().t1, 5.0f);
  ASSERT_EQ(s.record().t2, 5.0f);
}

TEST_F(Tsphere, raySphereNoIntersection) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 2.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_FALSE(s.intersect(r));
  ASSERT_EQ(s.record().count, 0);
  ASSERT_EQ(s.record().t1, 0.0f);
  ASSERT_EQ(s.record().t2, 0.0f);
}

TEST_F(Tsphere, rayOriginAtSphereCenterIntersection) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r));
  ASSERT_EQ(s.record().count, 2);
  ASSERT_EQ(s.record().t1, -1.0f);
  ASSERT_EQ(s.record().t2, 1.0f);
}

TEST_F(Tsphere, SphereIsBehindOrigin) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 0.0f, 5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  ASSERT_TRUE(s.intersect(r));
  ASSERT_EQ(s.record().count, 2);
  ASSERT_EQ(s.record().t1, -6.0f);
  ASSERT_EQ(s.record().t2, -4.0f);
}

TEST_F(Tsphere, testingTheNextTest) {
  s = Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  Ray r1 = r.transform(scale(2.0f, 2.0f, 2.0f).inverse());
  ASSERT_TRUE(s.intersect(r1));
  ASSERT_EQ(s.record().count, 2);
  ASSERT_EQ(s.record().t1, 3.0f);
  ASSERT_EQ(s.record().t2, 7.0f);
}

TEST_F(Tsphere, transformsSphere) {
  r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  Traceable *t = new Transformer(new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f),
                                 scale(2.0f, 2.0f, 2.0f));
  ASSERT_TRUE(t->intersect(r));
  ASSERT_EQ(t->record().count, 2);
  ASSERT_EQ(t->record().t1, 3.0f);
  ASSERT_EQ(t->record().t2, 7.0f);
  delete t;
}

TEST_F(Tsphere, translatesSphere) {
  r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  Traceable *t = new Transformer(new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f),
                                 translation(5.0f, 0.0f, 0.0f));
  ASSERT_FALSE(t->intersect(r));
  ASSERT_TRUE(t->record().count == 0);
  delete t;
}
