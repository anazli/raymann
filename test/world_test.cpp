#include "composite/world.h"

#include "builder/shape_builder.h"
#include "composite/properties.h"
#include "composite/scene_director.h"
#include "composite/traceable.h"
#include "geometry/sphere.h"
#include "gtest/gtest.h"

using namespace testing;
using std::make_shared;
using std::shared_ptr;
using std::vector;

class Tworld : public Test {
 public:
  SceneDirectorPtr direct;
  SceneDirectorPtr direct2;
  TraceableBuilderPtr builder;
  PointLight light;
  TraceablePtr w;
  Properties prop;

  Tworld()
      : light(PointLight(Point3f(-10.0f, 10.0f, -10.0f),
                         Vec3f(1.0f, 1.0f, 1.0f))) {
    w = make_shared<World>();
    builder = make_shared<ShapeBuilder>();
    w->setLight(light);
  }
};

TEST_F(Tworld, createsWorldOfShere) {
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s = direct->create(builder, prop);
  w->add(s);
  ASSERT_TRUE(s->getParent() == w.get());
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));

  ASSERT_TRUE(w->intersect(r));
  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, 4.0f);
  ASSERT_EQ(s->record().t2, 6.0f);

  Traceable &closest = w->closestHit(r);
  ASSERT_EQ(closest.record().t_min(), s->record().t_min());
  ASSERT_EQ(closest.record().t1, s->record().t1);
  ASSERT_EQ(closest.record().t2, s->record().t2);

  w->remove(s, false);
  ASSERT_FALSE(s->getParent() == w.get());
  ASSERT_TRUE(s->getParent() == nullptr);
}

TEST_F(Tworld, createsWorldOfTwoSpheres) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s = direct->create(builder, prop);
  prop.setSphereCenter(Point3f(0.0f, 0.0f, 5.0f))
      .setColor(Vec3f(0.09f, 0.172f, 0.909f))
      .setAmbient(0.f)
      .setDiffuse(0.f)
      .setSpecular(0.f)
      .setShininess(0.f);
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct2->create(builder, prop);

  w->add(s);
  w->add(s1);
  ASSERT_TRUE(s->getParent() == w.get());
  ASSERT_TRUE(s1->getParent() == w.get());
  ASSERT_TRUE(w->intersect(r));

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, 4.0f);
  ASSERT_EQ(s->record().t2, 6.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, 9.0f);
  ASSERT_EQ(s1->record().t2, 11.0f);

  Traceable &closest = w->closestHit(r);
  ASSERT_EQ(closest.record().t_min(), s->record().t_min());
  ASSERT_EQ(closest.record().t1, s->record().t1);
  ASSERT_EQ(closest.record().t2, s->record().t2);

  w->remove(s, false);
  w->remove(s1, false);

  ASSERT_FALSE(s->getParent() == w.get());
  ASSERT_TRUE(s->getParent() == nullptr);
  ASSERT_FALSE(s1->getParent() == w.get());
  ASSERT_TRUE(s1->getParent() == nullptr);
}

TEST_F(Tworld, createsWorldOfOneNegativeIntersection) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s = direct->create(builder, prop);
  prop.setSphereCenter(Point3f(0.0f, 0.0f, -8.0f))
      .setColor(Vec3f(0.09f, 0.172f, 0.909f))
      .setAmbient(0.f)
      .setDiffuse(0.f)
      .setSpecular(0.f)
      .setShininess(0.f);
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct2->create(builder, prop);

  w->add(s);
  w->add(s1);
  ASSERT_TRUE(w->intersect(r));

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, 4.0f);
  ASSERT_EQ(s->record().t2, 6.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, -4.0f);
  ASSERT_EQ(s1->record().t2, -2.0f);

  Traceable &closest = w->closestHit(r);
  ASSERT_EQ(closest.record().t_min(), s->record().t_min());
  ASSERT_EQ(closest.record().t1, s->record().t1);
  ASSERT_EQ(closest.record().t2, s->record().t2);
}

TEST_F(Tworld, createsWorldOfNegativeIntersections) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  direct = make_shared<StandardSphere>();
  prop.setSphereCenter(Point3f(0.0f, 0.0f, -11.0f))
      .setColor(Vec3f(0.09f, 0.172f, 0.909f))
      .setAmbient(0.f)
      .setDiffuse(0.f)
      .setSpecular(0.f)
      .setShininess(0.f);
  shared_ptr<Traceable> s = direct->create(builder, prop);
  prop.setSphereCenter(Point3f(0.0f, 0.0f, -8.0f));

  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct2->create(builder, prop);

  w->add(s);
  w->add(s1);
  ASSERT_TRUE(w->intersect(r));

  ASSERT_EQ(s->record().count, 2);
  ASSERT_EQ(s->record().t1, -7.0f);
  ASSERT_EQ(s->record().t2, -5.0f);
  ASSERT_EQ(s1->record().count, 2);
  ASSERT_EQ(s1->record().t1, -4.0f);
  ASSERT_EQ(s1->record().t2, -2.0f);

  // ASSERT_DEATH(Traceable &closest = w.closestHit(), "");  // Running it with
  // valgrind results in signal 6 (SIGABRT)
  Traceable &closest = w->closestHit(r);
  ASSERT_TRUE(&closest == s.get());
}

TEST_F(Tworld, createsWorldOfFourSpheres) {
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  prop.setSphereCenter(Point3f(0.0f, 0.0f, 5.0f))
      .setColor(Vec3f(0.09f, 0.172f, 0.909f))
      .setAmbient(0.f)
      .setDiffuse(0.f)
      .setSpecular(0.f)
      .setShininess(0.f);

  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s = direct->create(builder, prop);

  prop.setSphereCenter(Point3f(0.0f, 0.0f, 0.0f));
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct2->create(builder, prop);

  prop.setSphereCenter(Point3f(0.0f, 0.0f, -8.0f));
  SceneDirectorPtr direct3 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s2 = direct3->create(builder, prop);

  prop.setSphereCenter(Point3f(0.0f, 0.0f, 8.0f));
  SceneDirectorPtr direct4 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s3 = direct4->create(builder, prop);

  w->add(s);
  w->add(s1);
  w->add(s2);
  w->add(s3);
  ASSERT_TRUE(w->intersect(r));

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

  Traceable &closest = w->closestHit(r);
  ASSERT_EQ(closest.record().t_min(), s1->record().t_min());
  ASSERT_EQ(closest.record().t1, s1->record().t1);
  ASSERT_EQ(closest.record().t2, s1->record().t2);
}

TEST_F(Tworld, createsDefaultWorldForTheNextTests) {
  prop.setColor(Vec3f(0.8f, 1.0f, 0.6f))
      .setAmbient(0.1f)
      .setDiffuse(0.7f)
      .setSpecular(0.2f);
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  prop.setObjTrans(scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s2 = direct2->create(builder, prop);
  w->add(s1);
  w->add(s2);
  ASSERT_TRUE(s1->getParent() == w.get());
  ASSERT_TRUE(s2->getParent() == w.get());
}

TEST_F(Tworld, getsVectorOFIntersectionPoints) {
  prop.setColor(Vec3f(0.8f, 1.0f, 0.6f))
      .setAmbient(0.1f)
      .setDiffuse(0.7f)
      .setSpecular(0.2f);
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  prop.setObjTrans(scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s2 = direct2->create(builder, prop);
  w->add(s1);
  w->add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w->intersect(r);
  /*vector<float> v = w->intersectionsSorted();

  ASSERT_EQ(v.size(), 4);
  ASSERT_EQ(v[0], 4.0f);
  ASSERT_EQ(v[1], 4.5f);
  ASSERT_EQ(v[2], 5.5f);
  ASSERT_EQ(v[3], 6.0f);*/
}

TEST_F(Tworld, computesQuantitiesOfIntersection) {
  prop.setColor(Vec3f(0.8f, 1.0f, 0.6f))
      .setAmbient(0.1f)
      .setDiffuse(0.7f)
      .setSpecular(0.2f);
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  w->add(s1);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w->intersect(r);
  Traceable &t = w->closestHit(r);

  ASSERT_TRUE(t.record().eye(r) == Vec3f(0.0f, 0.0f, -1.0f));
  ASSERT_TRUE(t.record().point(r) == Point3f(0.0f, 0.0f, -1.0f));
  ASSERT_TRUE(t.normal(t.record().point(r)) == Vec3f(0.0f, 0.0f, -1.0f));
}

TEST_F(Tworld, intersectionWhenHitOccursOutside) {
  prop.setColor(Vec3f(0.8f, 1.0f, 0.6f))
      .setAmbient(0.1f)
      .setDiffuse(0.7f)
      .setSpecular(0.2f);
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  w->add(s1);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w->intersect(r);

  Traceable &t = w->closestHit(r);
  t.checkInside(r);

  ASSERT_FALSE(t.record().inside == true);
}

TEST_F(Tworld, intersectionWhenHitOccursInside) {
  prop.setColor(Vec3f(0.8f, 1.0f, 0.6f))
      .setAmbient(0.1f)
      .setDiffuse(0.7f)
      .setSpecular(0.2f);
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  w->add(s1);

  Ray r(Point3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w->intersect(r);

  Traceable &t = w->closestHit(r);
  t.checkInside(r);

  ASSERT_TRUE(t.record().inside == true);
}

TEST_F(Tworld, ShadingAnIntersection) {
  prop.setColor(Vec3f(0.8f, 1.0f, 0.6f))
      .setAmbient(0.1f)
      .setDiffuse(0.7f)
      .setSpecular(0.2f);
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  prop.setObjTrans(scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s2 = direct2->create(builder, prop);
  w->add(s1);
  w->add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w->intersect(r);

  Traceable &t = w->closestHit(r);
  t.checkInside(r);

  PointLight l(Point3f(-10.0f, 10.0f, -10.0f), Vec3f(1.0f, 1.0f, 1.0f));
  Vec3f color = t.lighting(w, r);

  ASSERT_TRUE(&t == s1.get());

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.38066f, eps);
  EXPECT_NEAR(color.y(), 0.47583f, eps);
  EXPECT_NEAR(color.z(), 0.2855f, eps);
}

TEST_F(Tworld, ShadingAnInsideIntersection) {
  prop.setColor(Vec3f(0.8f, 1.0f, 0.6f))
      .setAmbient(0.1f)
      .setDiffuse(0.7f)
      .setSpecular(0.2f);
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  prop.setColor(Vec3f(1.0f, 1.0f, 1.0f))
      .setAmbient(0.1f)
      .setDiffuse(0.9f)
      .setSpecular(0.9f)
      .setShininess(200.f)
      .setObjTrans(scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s2 = direct2->create(builder, prop);
  w->add(s1);
  w->add(s2);

  Ray r(Point3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w->intersect(r);

  Traceable &t = w->closestHit(r);
  t.checkInside(r);

  PointLight l(Point3f(0.0f, 0.25f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));
  Vec3f color = t.lighting(w, r);

  ASSERT_TRUE(&t == s2.get());

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.90498f, eps);
  EXPECT_NEAR(color.y(), 0.90498f, eps);
  EXPECT_NEAR(color.z(), 0.90498f, eps);
}

TEST_F(Tworld, colorWhenRayMisses) {
  prop.setColor(Vec3f(0.8f, 1.0f, 0.6f))
      .setAmbient(0.1f)
      .setDiffuse(0.7f)
      .setSpecular(0.2f);
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  prop.setColor(Vec3f(1.0f, 1.0f, 1.0f))
      .setAmbient(0.1f)
      .setDiffuse(0.9f)
      .setSpecular(0.9f)
      .setShininess(200.f)
      .setObjTrans(scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s2 = direct2->create(builder, prop);
  w->add(s1);
  w->add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 1.0f, 0.0f));
  w->intersect(r);

  Traceable &t = w->closestHit(r);

  PointLight l(Point3f(0.0f, 0.00f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f));
  w->setLight(l);
  Vec3f color = t.lighting(w, r);

  ASSERT_EQ(color.x(), 0.0f);
  ASSERT_EQ(color.y(), 0.0f);
  ASSERT_EQ(color.z(), 0.0f);
}

TEST_F(Tworld, colorWhenRayHits) {
  prop.setColor(Vec3f(0.8f, 1.0f, 0.6f))
      .setAmbient(0.1f)
      .setDiffuse(0.7f)
      .setSpecular(0.2f);
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  prop.setColor(Vec3f(1.0f, 1.0f, 1.0f))
      .setAmbient(0.1f)
      .setDiffuse(0.9f)
      .setSpecular(0.9f)
      .setShininess(200.f)
      .setObjTrans(scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s2 = direct2->create(builder, prop);
  w->add(s1);
  w->add(s2);

  Ray r(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  w->intersect(r);

  Traceable &t = w->closestHit(r);

  PointLight l(Point3f(-10.0f, 10.00f, -10.0f), Vec3f(1.0f, 1.0f, 1.0f));
  Vec3f color = t.lighting(w, r);

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.38066f, eps);
  EXPECT_NEAR(color.y(), 0.47583f, eps);
  EXPECT_NEAR(color.z(), 0.2855f, eps);
}

TEST_F(Tworld, colorWithAnIntersectionBehind) {
  prop.setColor(Vec3f(0.8f, 1.0f, 0.6f))
      .setAmbient(1.0f)
      .setDiffuse(0.7f)
      .setSpecular(0.2f);
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  prop.setColor(Vec3f(1.0f, 1.0f, 1.0f))
      .setAmbient(0.1f)
      .setDiffuse(0.9f)
      .setSpecular(0.9f)
      .setShininess(200.f)
      .setObjTrans(scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s2 = direct2->create(builder, prop);
  w->add(s1);
  w->add(s2);

  Ray r(Point3f(0.0f, 0.0f, 0.75f), Vec3f(0.0f, 0.0f, -1.0f));
  w->intersect(r);
  Traceable &t = w->closestHit(r);

  ASSERT_TRUE(&t == s2.get());  // color of the inner sphere
}

TEST_F(Tworld, noShadowWhenNothingCollinear) {
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  w->add(s1);
  Point3f p(0.0f, 10.0f, 0.0f);
  Ray r(p, (light.position() - p).normalize());
  w->intersect(r);
  Traceable &t = w->closestHit(r);
  ASSERT_FALSE(t.isShadowed(w, p));
}

TEST_F(Tworld, shadowWhenObjectBetweenLightAndPoint) {
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  w->add(s1);
  Point3f p(10.0f, -10.0f, 10.0f);
  Ray r(p, (light.position() - p).normalize());
  w->intersect(r);
  Traceable &t = w->closestHit(r);
  ASSERT_TRUE(t.isShadowed(w, p));
}

TEST_F(Tworld, noShadowWhenObjectBehindLight) {
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  w->add(s1);
  Point3f p(-20.0f, 20.0f, -20.0f);
  Ray r(p, (light.position() - p).normalize());
  w->intersect(r);
  Traceable &t = w->closestHit(r);
  ASSERT_FALSE(t.isShadowed(w, p));
}

TEST_F(Tworld, noShadowWhenObjectBehindPoint) {
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  w->add(s1);
  Point3f p(-2.0f, 2.0f, -2.0f);
  Ray r(p, (light.position() - p).normalize());
  w->intersect(r);
  Traceable &t = w->closestHit(r);
  ASSERT_FALSE(t.isShadowed(w, p));
}

TEST_F(Tworld, intersectionWithShadows) {
  direct = make_shared<StandardSphere>();
  shared_ptr<Traceable> s1 = direct->create(builder, prop);
  w->add(s1);
  prop.setObjTrans(transl(0.0f, 0.0f, 10.0f));
  direct2 = make_shared<StandardSphere>();
  shared_ptr<Traceable> s2 = direct2->create(builder, prop);
  w->add(s2);
  Ray r(Point3f(0.0f, 0.0f, 5.0f), Vec3f(0.0f, 0.0f, 1.0f));
  light = PointLight(Point3f(0.0f, 0.0f, 10.0f), Vec3f(1.0f, 1.0f, 1.0f));
  w->setLight(light);
  w->intersect(r);
  Traceable &t = w->closestHit(r);
  Vec3f color = t.lighting(w, r);
  ASSERT_TRUE(color == Vec3f(0.1f, 0.1f, 0.1f));
}
