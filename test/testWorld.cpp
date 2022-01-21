#include "geometry/sphere.h"
#include "geometry/traceable.h"
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
  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, 4.0f);
  ASSERT_EQ(s->record().t2, 6.0f);

  Traceable &closest = w.closestHit();
  ASSERT_EQ(closest.record().t_min(), s->record().t_min());
  ASSERT_EQ(closest.record().t1, s->record().t1);
  ASSERT_EQ(closest.record().t2, s->record().t2);

  w.remove(s, false);
  ASSERT_FALSE(s->getParent() == &w);
  ASSERT_TRUE(s->getParent() == nullptr);
  delete s;
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

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, 4.0f);
  ASSERT_EQ(s->record().t2, 6.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, 9.0f);
  ASSERT_EQ(s1->record().t2, 11.0f);

  Traceable &closest = w.closestHit();
  ASSERT_EQ(closest.record().t_min(), s->record().t_min());
  ASSERT_EQ(closest.record().t1, s->record().t1);
  ASSERT_EQ(closest.record().t2, s->record().t2);

  w.remove(s, false);
  w.remove(s1, false);

  ASSERT_FALSE(s->getParent() == &w);
  ASSERT_TRUE(s->getParent() == nullptr);
  ASSERT_FALSE(s1->getParent() == &w);
  ASSERT_TRUE(s1->getParent() == nullptr);
  delete s;
  delete s1;
}

TEST_F(Tworld, createsWorldOfOneNegativeIntersection) {
  w = World();
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  Traceable *s = new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  Traceable *s1 = new Sphere(Point3f(0.0f, 0.0f, -8.0f), 1.0f);

  w.add(s);
  w.add(s1);
  ASSERT_TRUE(w.intersect(r));

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, 4.0f);
  ASSERT_EQ(s->record().t2, 6.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, -4.0f);
  ASSERT_EQ(s1->record().t2, -2.0f);

  Traceable &closest = w.closestHit();
  ASSERT_EQ(closest.record().t_min(), s->record().t_min());
  ASSERT_EQ(closest.record().t1, s->record().t1);
  ASSERT_EQ(closest.record().t2, s->record().t2);
}

TEST_F(Tworld, createsWorldOfNegativeIntersections) {
  w = World();
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  Traceable *s = new Sphere(Point3f(0.0f, 0.0f, -11.0f), 1.0f);
  Traceable *s1 = new Sphere(Point3f(0.0f, 0.0f, -8.0f), 1.0f);

  w.add(s);
  w.add(s1);
  ASSERT_TRUE(w.intersect(r));

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, -7.0f);
  ASSERT_EQ(s->record().t2, -5.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, -4.0f);
  ASSERT_EQ(s1->record().t2, -2.0f);

  ASSERT_DEATH(Traceable &closest = w.closestHit(), "");  // Running it with
}  // valgrind results in signal 6 (SIGABRT)

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

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, 9.0f);
  ASSERT_EQ(s->record().t2, 11.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, 4.0f);
  ASSERT_EQ(s1->record().t2, 6.0f);
  ASSERT_EQ(s2->record().count, 2);
  ASSERT_EQ(s2->record().t1, -4.0f);
  ASSERT_EQ(s2->record().t2, -2.0f);
  ASSERT_EQ(s3->record().count, 2);
  ASSERT_EQ(s3->record().t1, 12.0f);
  ASSERT_EQ(s3->record().t2, 14.0f);

  Traceable &closest = w.closestHit();
  ASSERT_EQ(closest.record().t_min(), s1->record().t_min());
  ASSERT_EQ(closest.record().t1, s1->record().t1);
  ASSERT_EQ(closest.record().t2, s1->record().t2);
}

TEST_F(Tworld, createsDefaultWorldForTheNextTests) {
  w = World();
  Traceable *s1 =
      new Material(new Sphere(), Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  Traceable *s2 = new Transformer(new Sphere(), scale(0.5f, 0.5f, 0.5f));
  w.add(s1);
  w.add(s2);
  ASSERT_TRUE(s1->getParent() == &w);
  ASSERT_TRUE(s2->getParent() == &w);
}

TEST_F(Tworld, getsVectorOFIntersectionPoints) {
  w = World();
  Traceable *s1 =
      new Material(new Sphere(), Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  Traceable *s2 = new Transformer(new Sphere(), scale(0.5f, 0.5f, 0.5f));
  w.add(s1);
  w.add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w.intersect(r);
  std::vector<float> v = w.intersectionsSorted();

  ASSERT_EQ(v.size(), 4);
  ASSERT_EQ(v[0], 4.0f);
  ASSERT_EQ(v[1], 4.5f);
  ASSERT_EQ(v[2], 5.5f);
  ASSERT_EQ(v[3], 6.0f);
}

TEST_F(Tworld, computesQuantitiesOfIntersection) {
  w = World();
  Traceable *s1 =
      new Material(new Sphere(), Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  w.add(s1);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w.intersect(r);
  Traceable &t = w.closestHit();

  ASSERT_TRUE(t.record().eye(r) == Vec3f(0.0f, 0.0f, -1.0f));
  ASSERT_TRUE(t.record().point(r) == Point3f(0.0f, 0.0f, -1.0f));
  ASSERT_TRUE(t.normal(t.record().point(r)) == Vec3f(0.0f, 0.0f, -1.0f));
}
