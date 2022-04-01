#include "geometry/sphere.h"
#include "geometry/traceable.h"
#include "gtest/gtest.h"
#include "world/scene.h"
#include "world/world.h"

using namespace testing;
using std::shared_ptr;
using std::vector;

class Tworld : public Test {
 public:
  Scene scene;
  World w;
  std::shared_ptr<SphereBuilder> builder;

  Tworld() : w(World()), builder(new StandardSphere) {}
};

TEST_F(Tworld, createsWorldOfShere) {
  shared_ptr<Traceable> s = scene.createSphere(builder);
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
}

TEST_F(Tworld, createsWorldOfTwoSpheres) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  shared_ptr<Traceable> s = scene.createSphere(builder);
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.09f, 0.172f, 0.909f), 0, 0, 0, 0,
                         Point3f(0.0f, 0.0f, 5.0f), 1.0f);

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
}

TEST_F(Tworld, createsWorldOfOneNegativeIntersection) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  shared_ptr<Traceable> s = scene.createSphere(builder);
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.09f, 0.172f, 0.909f), 0, 0, 0, 0,
                         Point3f(0.0f, 0.0f, -8.0f), 1.0f);

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
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  shared_ptr<Traceable> s =
      scene.createSphere(builder, Vec3f(0.09f, 0.172f, 0.909f), 0, 0, 0, 0,
                         Point3f(0.0f, 0.0f, -11.0f), 1.0f);
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.09f, 0.172f, 0.909f), 0, 0, 0, 0,
                         Point3f(0.0f, 0.0f, -8.0f), 1.0f);

  w.add(s);
  w.add(s1);
  ASSERT_TRUE(w.intersect(r));

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, -7.0f);
  ASSERT_EQ(s->record().t2, -5.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, -4.0f);
  ASSERT_EQ(s1->record().t2, -2.0f);

  // ASSERT_DEATH(Traceable &closest = w.closestHit(), "");  // Running it with
  // valgrind results in signal 6 (SIGABRT)
  Traceable &closest = w.closestHit();
  ASSERT_TRUE(&closest == s.get());
}
TEST_F(Tworld, createsWorldOfFourSpheres) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  shared_ptr<Traceable> s =
      scene.createSphere(builder, Vec3f(0.09f, 0.172f, 0.909f), 0, 0, 0, 0,
                         Point3f(0.0f, 0.0f, 5.0f), 1.0f);
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.09f, 0.172f, 0.909f), 0, 0, 0, 0,
                         Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  shared_ptr<Traceable> s2 =
      scene.createSphere(builder, Vec3f(0.09f, 0.172f, 0.909f), 0, 0, 0, 0,
                         Point3f(0.0f, 0.0f, -8.0f), 1.0f);
  shared_ptr<Traceable> s3 =
      scene.createSphere(builder, Vec3f(0.09f, 0.172f, 0.909f), 0, 0, 0, 0,
                         Point3f(0.0f, 0.0f, 8.0f), 1.0f);

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
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  shared_ptr<Traceable> s2 =
      scene.createTransformedSphere(builder, scale(0.5f, 0.5f, 0.5f));
  w.add(s1);
  w.add(s2);
  ASSERT_TRUE(s1->getParent() == &w);
  ASSERT_TRUE(s2->getParent() == &w);
}

TEST_F(Tworld, getsVectorOFIntersectionPoints) {
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  shared_ptr<Traceable> s2 =
      scene.createTransformedSphere(builder, scale(0.5f, 0.5f, 0.5f));
  w.add(s1);
  w.add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w.intersect(r);
  vector<float> v = w.intersectionsSorted();

  ASSERT_EQ(v.size(), 4);
  ASSERT_EQ(v[0], 4.0f);
  ASSERT_EQ(v[1], 4.5f);
  ASSERT_EQ(v[2], 5.5f);
  ASSERT_EQ(v[3], 6.0f);
}

TEST_F(Tworld, computesQuantitiesOfIntersection) {
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  w.add(s1);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w.intersect(r);
  Traceable &t = w.closestHit();

  ASSERT_TRUE(t.record().eye(r) == Vec3f(0.0f, 0.0f, -1.0f));
  ASSERT_TRUE(t.record().point(r) == Point3f(0.0f, 0.0f, -1.0f));
  ASSERT_TRUE(t.normal(t.record().point(r)) == Vec3f(0.0f, 0.0f, -1.0f));
}

TEST_F(Tworld, intersectionWhenHitOccursOutside) {
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  w.add(s1);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w.intersect(r);

  Traceable &t = w.closestHit();
  t.checkInside(r);

  ASSERT_FALSE(t.record().inside == true);
}

TEST_F(Tworld, intersectionWhenHitOccursInside) {
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  w.add(s1);

  Ray r(Point3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w.intersect(r);

  Traceable &t = w.closestHit();
  t.checkInside(r);

  ASSERT_TRUE(t.record().inside == true);
}

TEST_F(Tworld, ShadingAnIntersection) {
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  shared_ptr<Traceable> s2 =
      scene.createTransformedSphere(builder, scale(0.5f, 0.5f, 0.5f));
  w.add(s1);
  w.add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w.intersect(r);

  Traceable &t = w.closestHit();
  t.checkInside(r);

  PointLight l(Point3f(-10.0f, 10.0f, -10.0f), Vec3f(1.0f, 1.0f, 1.0f));
  Vec3f color = t.lighting(l, r);

  ASSERT_TRUE(&t == s1.get());

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.38066f, eps);
  EXPECT_NEAR(color.y(), 0.47583f, eps);
  EXPECT_NEAR(color.z(), 0.2855f, eps);
}

TEST_F(Tworld, ShadingAnInsideIntersection) {
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  shared_ptr<Traceable> s2 = scene.createTransformedSphere(
      builder, scale(0.5f, 0.5f, 0.5f), Vec3f(1.0f, 1.0f, 1.0f), 0.1f, 0.9f,
      0.9f, 200.0f);
  w.add(s1);
  w.add(s2);

  Ray r(Point3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w.intersect(r);

  Traceable &t = w.closestHit();
  t.checkInside(r);

  PointLight l(Point3f(0.0f, 0.25f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));
  Vec3f color = t.lighting(l, r);

  ASSERT_TRUE(&t == s2.get());

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.90498f, eps);
  EXPECT_NEAR(color.y(), 0.90498f, eps);
  EXPECT_NEAR(color.z(), 0.90498f, eps);
}

TEST_F(Tworld, colorWhenRayMisses) {
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  shared_ptr<Traceable> s2 = scene.createTransformedSphere(
      builder, scale(0.5f, 0.5f, 0.5f), Vec3f(1.0f, 1.0f, 1.0f), 0.1f, 0.9f,
      0.9f, 200.0f);
  w.add(s1);
  w.add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 1.0f, 0.0f));
  w.intersect(r);

  Traceable &t = w.closestHit();

  PointLight l(Point3f(0.0f, 0.00f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f));
  Vec3f color = t.lighting(l, r);

  ASSERT_EQ(color.x(), 0.0f);
  ASSERT_EQ(color.y(), 0.0f);
  ASSERT_EQ(color.z(), 0.0f);
}

TEST_F(Tworld, colorWhenRayHits) {
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.8f, 1.0f, 0.6f), 0.1f, 0.7f, 0.2f);
  shared_ptr<Traceable> s2 = scene.createTransformedSphere(
      builder, scale(0.5f, 0.5f, 0.5f), Vec3f(1.0f, 1.0f, 1.0f), 0.1f, 0.9f,
      0.9f, 200.0f);
  w.add(s1);
  w.add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w.intersect(r);

  Traceable &t = w.closestHit();

  PointLight l(Point3f(-10.0f, 10.00f, -10.0f), Vec3f(1.0f, 1.0f, 1.0f));
  Vec3f color = t.lighting(l, r);

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.38066f, eps);
  EXPECT_NEAR(color.y(), 0.47583f, eps);
  EXPECT_NEAR(color.z(), 0.2855f, eps);
}

TEST_F(Tworld, colorWithAnIntersectionBehind) {
  shared_ptr<Traceable> s1 =
      scene.createSphere(builder, Vec3f(0.8f, 1.0f, 0.6f), 1.0f, 0.7f, 0.2f);
  shared_ptr<Traceable> s2 = scene.createTransformedSphere(
      builder, scale(0.5f, 0.5f, 0.5f), Vec3f(1.0f, 1.0f, 1.0f), 1.0f, 0.9f,
      0.9f, 200.0f);
  w.add(s1);
  w.add(s2);

  Ray r(Point3f(0.0f, 0.0f, 0.75f), Vec3f(0.0f, 0.0f, -1.0f));
  w.intersect(r);
  Traceable &t = w.closestHit();

  ASSERT_TRUE(&t == s2.get());  // color of the inner sphere
}
