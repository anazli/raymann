#include "geometry/sphere.h"
#include "geometry/traceable.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace testing;

class Tworld : public Test {
 public:
  World w;
};

TEST_F(Tworld, createsWorldOfShere) {
  w = World();
  Traceable *s = new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  w.add(s);
  ASSERT_TRUE(s->getParent() == &w);
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));

  ASSERT_TRUE(w.intersect(r));
  ASSERT_EQ(s->rec.count, 2);
  ASSERT_EQ(s->rec.t1, 4.0f);
  ASSERT_EQ(s->rec.t2, 6.0f);

  Traceable &closest = w.closestHit();
  ASSERT_EQ(closest.rec.t_min(), s->rec.t_min());
  ASSERT_EQ(closest.rec.t1, s->rec.t1);
  ASSERT_EQ(closest.rec.t2, s->rec.t2);

  w.remove(s);
  ASSERT_FALSE(s->getParent() == &w);
  ASSERT_TRUE(s->getParent() == nullptr);
}

TEST_F(Tworld, createsWorldOfTwoSpheres) {
  w = World();
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  Traceable *s = new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  Traceable *s1 = new Sphere(Point3f(0.0f, 0.0f, 5.0f), 1.0f);

  w.add(s);
  w.add(s1);
  ASSERT_TRUE(s->getParent() == &w);
  ASSERT_TRUE(s1->getParent() == &w);
  ASSERT_TRUE(w.intersect(r));

  ASSERT_EQ(s->rec.count, 2);
  ASSERT_EQ(s->rec.t1, 4.0f);
  ASSERT_EQ(s->rec.t2, 6.0f);
  ASSERT_EQ(s1->rec.count, 2);
  ASSERT_EQ(s1->rec.t1, 9.0f);
  ASSERT_EQ(s1->rec.t2, 11.0f);

  Traceable &closest = w.closestHit();
  ASSERT_EQ(closest.rec.t_min(), s->rec.t_min());
  ASSERT_EQ(closest.rec.t1, s->rec.t1);
  ASSERT_EQ(closest.rec.t2, s->rec.t2);

  w.remove(s);
  w.remove(s1);

  ASSERT_FALSE(s->getParent() == &w);
  ASSERT_TRUE(s->getParent() == nullptr);
  ASSERT_FALSE(s1->getParent() == &w);
  ASSERT_TRUE(s1->getParent() == nullptr);
}

TEST_F(Tworld, createsWorldOfOneNegativeIntersection) {
  w = World();
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  Traceable *s = new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  Traceable *s1 = new Sphere(Point3f(0.0f, 0.0f, -8.0f), 1.0f);

  w.add(s);
  w.add(s1);
  ASSERT_TRUE(w.intersect(r));

  ASSERT_EQ(s->rec.count, 2);
  ASSERT_EQ(s->rec.t1, 4.0f);
  ASSERT_EQ(s->rec.t2, 6.0f);
  ASSERT_EQ(s1->rec.count, 2);
  ASSERT_EQ(s1->rec.t1, -4.0f);
  ASSERT_EQ(s1->rec.t2, -2.0f);

  Traceable &closest = w.closestHit();
  ASSERT_EQ(closest.rec.t_min(), s->rec.t_min());
  ASSERT_EQ(closest.rec.t1, s->rec.t1);
  ASSERT_EQ(closest.rec.t2, s->rec.t2);
}

TEST_F(Tworld, createsWorldOfNegativeIntersections) {
  w = World();
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  Traceable *s = new Sphere(Point3f(0.0f, 0.0f, -11.0f), 1.0f);
  Traceable *s1 = new Sphere(Point3f(0.0f, 0.0f, -8.0f), 1.0f);

  w.add(s);
  w.add(s1);
  ASSERT_TRUE(w.intersect(r));

  ASSERT_EQ(s->rec.count, 2);
  ASSERT_EQ(s->rec.t1, -7.0f);
  ASSERT_EQ(s->rec.t2, -5.0f);
  ASSERT_EQ(s1->rec.count, 2);
  ASSERT_EQ(s1->rec.t1, -4.0f);
  ASSERT_EQ(s1->rec.t2, -2.0f);

  ASSERT_DEATH(Traceable &closest = w.closestHit(), "");
}

TEST_F(Tworld, createsWorldOfFourSpheres) {
  w = World();
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  Traceable *s = new Sphere(Point3f(0.0f, 0.0f, 5.0f), 1.0f);
  Traceable *s1 = new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  Traceable *s2 = new Sphere(Point3f(0.0f, 0.0f, -8.0f), 1.0f);
  Traceable *s3 = new Sphere(Point3f(0.0f, 0.0f, 8.0f), 1.0f);

  w.add(s);
  w.add(s1);
  w.add(s2);
  w.add(s3);
  ASSERT_TRUE(w.intersect(r));

  ASSERT_EQ(s->rec.count, 2);
  ASSERT_EQ(s->rec.t1, 9.0f);
  ASSERT_EQ(s->rec.t2, 11.0f);
  ASSERT_EQ(s1->rec.count, 2);
  ASSERT_EQ(s1->rec.t1, 4.0f);
  ASSERT_EQ(s1->rec.t2, 6.0f);
  ASSERT_EQ(s2->rec.count, 2);
  ASSERT_EQ(s2->rec.t1, -4.0f);
  ASSERT_EQ(s2->rec.t2, -2.0f);
  ASSERT_EQ(s3->rec.count, 2);
  ASSERT_EQ(s3->rec.t1, 12.0f);
  ASSERT_EQ(s3->rec.t2, 14.0f);

  Traceable &closest = w.closestHit();
  ASSERT_EQ(closest.rec.t_min(), s1->rec.t_min());
  ASSERT_EQ(closest.rec.t1, s1->rec.t1);
  ASSERT_EQ(closest.rec.t2, s1->rec.t2);
}
