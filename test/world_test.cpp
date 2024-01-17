#include "composite/world.h"

#include "composite/builder.h"
#include "composite/scene_element.h"
#include "composite/builder.h"
#include "geometry/sphere.h"
#include "gtest/gtest.h"

using namespace testing;
using std::make_shared;
using std::shared_ptr;
using std::vector;
using std::make_unique;

class Tworld : public Test {
 public:
  BuilderPtr builder;
  PointLight light;
  SceneElementPtr world;
  MaterialProperties prop;
  IntersectionRecord rec;

  //Tworld()
  //    : light(PointLight(Point3f(-10.0f, 10.0f, -10.0f),
  //                       Vec3f(1.0f, 1.0f, 1.0f))) {
  //}
};

TEST_F(Tworld, createsWorldOfShere) {
  builder = make_unique<WorldBuilder>();
  builder->createWorld(light);
  builder->createSphere();
  SceneElementRawPtr sphere = builder->getCurrentElement();
  builder->addElement();
  world = builder->getProduct();
  ASSERT_TRUE(sphere->getParent() == world.get());
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));

  ASSERT_TRUE(world->intersect(r, rec));
  ASSERT_EQ(rec.count, 2);
  ASSERT_EQ(rec.t1, 4.0f);
  ASSERT_EQ(rec.t2, 6.0f);

// TODO: check remove's behaviour
  world->remove(sphere, false);
  ASSERT_FALSE(sphere->getParent() == world.get());
  ASSERT_TRUE(sphere->getParent() == nullptr);
}

TEST_F(Tworld, createsWorldOfTwoSpheres) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  direct = make_shared<StandardSphere>();
  TraceablePtr s = direct->create(builder, prop);
  prop.setProperty(Props::SPHERE_CENTER, Point3f(0.0f, 0.0f, 5.0f))
      .setProperty(Props::COLOR, Vec3f(0.09f, 0.172f, 0.909f))
      .setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.f)
      .setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::SHININESS, 0.f);
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s1 = direct2->create(builder, prop);

  world->add(s);
  world->add(s1);
  ASSERT_TRUE(s->getParent() == world);
  ASSERT_TRUE(s1->getParent() == world);
  ASSERT_TRUE(world->intersect(r));

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, 4.0f);
  ASSERT_EQ(s->record().t2, 6.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, 9.0f);
  ASSERT_EQ(s1->record().t2, 11.0f);

  TraceablePtr closest = world->closestHit(r);
  ASSERT_EQ(closest->record().t_min(), s->record().t_min());
  ASSERT_EQ(closest->record().t1, s->record().t1);
  ASSERT_EQ(closest->record().t2, s->record().t2);

  world->remove(s, false);
  world->remove(s1, false);

  ASSERT_FALSE(s->getParent() == world);
  ASSERT_TRUE(s->getParent() == nullptr);
  ASSERT_FALSE(s1->getParent() == world);
  ASSERT_TRUE(s1->getParent() == nullptr);
}

TEST_F(Tworld, createsWorldOfOneNegativeIntersection) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  direct = make_shared<StandardSphere>();
  TraceablePtr s = direct->create(builder, prop);
  prop.setProperty(Props::SPHERE_CENTER, Point3f(0.0f, 0.0f, -8.0f))
      .setProperty(Props::COLOR, Vec3f(0.09f, 0.172f, 0.909f))
      .setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.f)
      .setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::SHININESS, 0.f);
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s1 = direct2->create(builder, prop);

  world->add(s);
  world->add(s1);
  ASSERT_TRUE(world->intersect(r));

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, 4.0f);
  ASSERT_EQ(s->record().t2, 6.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, -4.0f);
  ASSERT_EQ(s1->record().t2, -2.0f);

  TraceablePtr closest = world->closestHit(r);
  ASSERT_EQ(closest->record().t_min(), s->record().t_min());
  ASSERT_EQ(closest->record().t1, s->record().t1);
  ASSERT_EQ(closest->record().t2, s->record().t2);
}

TEST_F(Tworld, createsWorldOfNegativeIntersections) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  direct = make_shared<StandardSphere>();
  prop.setProperty(Props::SPHERE_CENTER, Point3f(0.0f, 0.0f, -11.0f))
      .setProperty(Props::COLOR, Vec3f(0.09f, 0.172f, 0.909f))
      .setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.f)
      .setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::SHININESS, 0.f);
  TraceablePtr s = direct->create(builder, prop);
  prop.setProperty(Props::SPHERE_CENTER, Point3f(0.0f, 0.0f, -8.0f));

  direct2 = make_shared<StandardSphere>();
  TraceablePtr s1 = direct2->create(builder, prop);

  world->add(s);
  world->add(s1);
  ASSERT_TRUE(world->intersect(r));

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, -7.0f);
  ASSERT_EQ(s->record().t2, -5.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, -4.0f);
  ASSERT_EQ(s1->record().t2, -2.0f);

  // ASSERT_DEATH(Traceable &closest = w.closestHit(), "");  // Running it with
  // valgrind results in signal 6 (SIGABRT)
  TraceablePtr closest = world->closestHit(r);
  ASSERT_TRUE(closest == s);
}

TEST_F(Tworld, createsWorldOfFourSpheres) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  prop.setProperty(Props::SPHERE_CENTER, Point3f(0.f, 0.f, 5.f))
      .setProperty(Props::COLOR, Vec3f(0.09f, 0.172f, 0.909f))
      .setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.f)
      .setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::SHININESS, 0.f);

  direct = make_shared<StandardSphere>();
  TraceablePtr s = direct->create(builder, prop);

  prop.setProperty(Props::SPHERE_CENTER, Point3f(0.0f, 0.0f, 0.0f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s1 = direct2->create(builder, prop);

  prop.setProperty(Props::SPHERE_CENTER, Point3f(0.0f, 0.0f, -8.0f));
  SceneDirectorPtr direct3 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct3->create(builder, prop);

  prop.setProperty(Props::SPHERE_CENTER, Point3f(0.0f, 0.0f, 8.0f));
  SceneDirectorPtr direct4 = make_shared<StandardSphere>();
  TraceablePtr s3 = direct4->create(builder, prop);

  world->add(s);
  world->add(s1);
  world->add(s2);
  world->add(s3);
  ASSERT_TRUE(world->intersect(r));

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

  TraceablePtr closest = world->closestHit(r);
  ASSERT_EQ(closest->record().t_min(), s1->record().t_min());
  ASSERT_EQ(closest->record().t1, s1->record().t1);
  ASSERT_EQ(closest->record().t2, s1->record().t2);
}

TEST_F(Tworld, createsDefaultWorldForTheNextTests) {
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);
  ASSERT_TRUE(s1->getParent() == world);
  ASSERT_TRUE(s2->getParent() == world);
}

TEST_F(Tworld, getsVectorOFIntersectionPoints) {
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  world->intersect(r);
  /*vector<float> v = w->intersectionsSorted();

  ASSERT_EQ(v.size(), 4);
  ASSERT_EQ(v[0], 4.0f);
  ASSERT_EQ(v[1], 4.5f);
  ASSERT_EQ(v[2], 5.5f);
  ASSERT_EQ(v[3], 6.0f);*/
}

TEST_F(Tworld, computesQuantitiesOfIntersection) {
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);

  ASSERT_TRUE(t->record().eye(r) == Vec3f(0.0f, 0.0f, -1.0f));
  ASSERT_TRUE(t->record().point(r) == Point3f(0.0f, 0.0f, -1.0f));
  ASSERT_TRUE(t->normal(t->record().point(r)) == Vec3f(0.0f, 0.0f, -1.0f));
}

TEST_F(Tworld, intersectionWhenHitOccursOutside) {
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  world->intersect(r);

  TraceablePtr t = world->closestHit(r);
  t->checkInside(r);

  ASSERT_FALSE(t->record().inside == true);
}

TEST_F(Tworld, intersectionWhenHitOccursInside) {
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);

  Ray r(Point3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  world->intersect(r);

  TraceablePtr t = world->closestHit(r);
  t->checkInside(r);

  ASSERT_TRUE(t->record().inside == true);
}

TEST_F(Tworld, ShadingAnIntersection) {
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  world->intersect(r);

  TraceablePtr t = world->closestHit(r);
  t->checkInside(r);

  PointLight l(Point3f(-10.0f, 10.0f, -10.0f), Vec3f(1.0f, 1.0f, 1.0f));
  t->setLight(l);
  Vec3f color = t->lighting(r);

  ASSERT_TRUE(t == s1);

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.38066f, eps);
  EXPECT_NEAR(color.y(), 0.47583f, eps);
  EXPECT_NEAR(color.z(), 0.2855f, eps);
}

TEST_F(Tworld, ShadingAnInsideIntersection) {
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::COLOR, Vec3f(1.0f, 1.0f, 1.0f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.9f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::SHININESS, 200.f)
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);

  Ray r(Point3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  world->intersect(r);

  TraceablePtr t = world->closestHit(r);
  t->checkInside(r);

  PointLight l(Point3f(0.0f, 0.25f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));
  t->setLight(l);
  Vec3f color = t->lighting(r);

  ASSERT_TRUE(t == s2);

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.90498f, eps);
  EXPECT_NEAR(color.y(), 0.90498f, eps);
  EXPECT_NEAR(color.z(), 0.90498f, eps);
}

TEST_F(Tworld, colorWhenRayMisses) {
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::COLOR, Vec3f(1.0f, 1.0f, 1.0f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.9f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::SHININESS, 200.f)
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 1.0f, 0.0f));
  world->intersect(r);

  PointLight l(Point3f(0.0f, 0.00f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f));
  world->setLight(l);
  TraceablePtr t = world->closestHit(r);
  ASSERT_FALSE(t);
}

TEST_F(Tworld, colorWhenRayHits) {
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::COLOR, Vec3f(1.0f, 1.0f, 1.0f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.9f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::SHININESS, 200.f)
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  world->intersect(r);

  TraceablePtr t = world->closestHit(r);

  PointLight l(Point3f(-10.0f, 10.00f, -10.0f), Vec3f(1.0f, 1.0f, 1.0f));
  t->setLight(l);
  Vec3f color = t->lighting(r);

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.38066f, eps);
  EXPECT_NEAR(color.y(), 0.47583f, eps);
  EXPECT_NEAR(color.z(), 0.2855f, eps);
}

TEST_F(Tworld, colorWithAnIntersectionBehind) {
  prop.setProperty(Props::COLOR, Vec3f(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 1.0f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::COLOR, Vec3f(1.0f, 1.0f, 1.0f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.9f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::SHININESS, 200.f)
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);

  Ray r(Point3f(0.0f, 0.0f, 0.75f), Vec3f(0.0f, 0.0f, -1.0f));
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);

  ASSERT_TRUE(t == s2);  // color of the inner sphere
}

TEST_F(Tworld, noShadowWhenNothingCollinear) {
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);
  Point3f p(0.0f, 10.0f, 0.0f);
  Ray r(p, (light.position() - p).normalize());
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);
  ASSERT_FALSE(t);  // No hit -> no shadow
}

TEST_F(Tworld, shadowWhenObjectBetweenLightAndPoint) {
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);
  Point3f p(10.0f, -10.0f, 10.0f);
  Ray r(p, (light.position() - p).normalize());
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);
  ASSERT_TRUE(t->isShadowed(p));
}

TEST_F(Tworld, noShadowWhenObjectBehindLight) {
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);
  Point3f p(-20.0f, 20.0f, -20.0f);
  Ray r(p, (light.position() - p).normalize());
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);
  ASSERT_FALSE(t->isShadowed(p));
}

TEST_F(Tworld, noShadowWhenObjectBehindPoint) {
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);
  Point3f p(-2.0f, 2.0f, -2.0f);
  Ray r(p, (light.position() - p).normalize());
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);
  ASSERT_FALSE(t);  // No hit -> now shadow
}

TEST_F(Tworld, intersectionWithShadows) {
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);
  prop.setProperty(Props::OBJECT_TRANSFROM_MATRIX, transl(0.0f, 0.0f, 10.0f));
  direct2 = make_shared<StandardSphere>();
  shared_ptr<SceneElement> s2 = direct2->create(builder, prop);
  world->add(s2);
  Ray r(Point3f(0.0f, 0.0f, 5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  light = PointLight(Point3f(0.0f, 0.0f, 10.0f), Vec3f(1.0f, 1.0f, 1.0f));
  world->setLight(light);
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);
  t->setLight(light);
  Vec3f color = t->lighting(r);
  ASSERT_TRUE(color == Vec3f(0.1f, 0.1f, 0.1f));
}
