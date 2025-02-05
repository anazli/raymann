#include "composite/builder.h"
#include "composite/scene_element.h"
#include "geometry/sphere.h"
#include "gtesting.h"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::vector;

class WorldTest : public testing::RTest {
 public:
  BuilderPtr builder;
  PointLight light;
  SceneElementPtr world;
  MaterialProperties prop;
  IntersectionRecord rec;
};

TEST_F(WorldTest, createsWorldOfShere) {
  builder = make_unique<WorldBuilder>();
  builder->addLight(light);
  builder->createWorld();
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(Mat4D());
  SceneElementRawPtr sphere = builder->getCurrentElement();
  EXPECT_TRUE(sphere->getParent() == nullptr);
  builder->addElement();
  world = builder->getProduct();
  EXPECT_TRUE(sphere->getParent() == world.get());
  Ray r = Ray(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));

  EXPECT_TRUE(world->intersect(r, rec));
  EXPECT_EQ(rec.count, 2);
  EXPECT_EQ(rec.t1, 4.0f);
  EXPECT_EQ(rec.t2, 6.0f);
}

TEST_F(WorldTest, createsWorldOfTwoSpheres) {
  Ray r = Ray(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  builder = make_unique<WorldBuilder>();
  builder->addLight(light);
  builder->createWorld();
  builder->processSceneElement(new Sphere(Point3D(0.0f, 0.0f, 5.0f)));
  SceneElementRawPtr s = builder->getCurrentElement();
  builder->addElement();
  builder->processSceneElement(new Sphere);
  SceneElementRawPtr s1 = builder->getCurrentElement();
  builder->addElement();
  world = builder->getProduct();

  EXPECT_TRUE(s->getParent() == world.get());
  EXPECT_TRUE(s1->getParent() == world.get());

  EXPECT_TRUE(world->intersect(r, rec));

  EXPECT_EQ(rec.count, 2);
  EXPECT_EQ(rec.t1, 4.0f);
  EXPECT_EQ(rec.t2, 6.0f);
}

TEST_F(WorldTest, whenWorldIsTranformed_parentOfChildIsCorrect) {
  builder = make_unique<WorldBuilder>();
  builder->addLight(light);
  builder->createWorld();
  builder->applyWorldTransformation(Mat4D());
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(Mat4D());
  SceneElementRawPtr sphere = builder->getCurrentElement();
  EXPECT_TRUE(sphere->getParent() == nullptr);
  builder->addElement();
  world = builder->getProduct();
  EXPECT_TRUE(sphere->getParent() == world.get());
}

TEST_F(WorldTest, intersectingTransformedWorld) {
  Ray r = Ray(Point3D(10.0f, 0.0f, -10.0f), Vec3D(0.0f, 0.0f, 1.0f));
  builder = make_unique<WorldBuilder>();
  builder->addLight(light);
  builder->createWorld();

  builder->processSceneElement(new Sphere);
  builder->applyTransformation(scale(2.f, 2.f, 2.f));
  builder->addElement();
  builder->applyWorldTransformation(translation(5.f, 0.f, 0.f));

  SceneElementPtr world = builder->getProduct();
  std::cout << (*world->getChildren().begin())->boundingBox().minPoint()
            << std::endl;
  std::cout << (*world->getChildren().begin())->boundingBox().maxPoint()
            << std::endl;
  std::cout << world->boundingBox().minPoint() << std::endl;
  std::cout << world->boundingBox().maxPoint() << std::endl;
  EXPECT_TRUE(world->intersect(r, rec));
  EXPECT_EQ(rec.count, 2);
}

TEST_F(WorldTest, convertingPointFromWorldToObjectSpace) {
  builder = make_unique<WorldBuilder>();
  builder->addLight(light);

  builder->createWorld();
  builder->applyWorldTransformation(rotationOverY(PI / 2.f));
  SceneElementPtr outerWorld = builder->getProduct();

  builder->createWorld();
  builder->applyWorldTransformation(scale(2.f, 2.f, 2.f));
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(5.f, 0.f, 0.f));
  builder->addElement();
  SceneElementPtr innerWorld = builder->getProduct();

  SceneElementRawPtr sphere = builder->getCurrentElement();
  outerWorld->add(innerWorld);

  EXPECT_EQ(sphere->getParent(), innerWorld.get());
  EXPECT_EQ(innerWorld->getParent(), outerWorld.get());
  EXPECT_EQ(outerWorld->getParent(), nullptr);

  Point3D p(-2.f, 0.f, -10.f);
  Point3D point = sphere->pointFromWorldToObjectSpace(p);
  comparePoints(point, Point3D(0.f, 0.f, -1.f));
}

TEST_F(WorldTest, convertingVectorFromObjectToWorldSpace) {
  builder = make_unique<WorldBuilder>();
  builder->addLight(light);

  builder->createWorld();
  builder->applyWorldTransformation(rotationOverY(PI / 2.f));
  SceneElementPtr outerWorld = builder->getProduct();

  builder->createWorld();
  builder->applyWorldTransformation(scale(1.f, 2.f, 3.f));
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(5.f, 0.f, 0.f));
  builder->addElement();
  SceneElementPtr innerWorld = builder->getProduct();

  SceneElementRawPtr sphere = builder->getCurrentElement();
  outerWorld->add(innerWorld);

  EXPECT_EQ(sphere->getParent(), innerWorld.get());
  EXPECT_EQ(innerWorld->getParent(), outerWorld.get());
  EXPECT_EQ(outerWorld->getParent(), nullptr);

  Vec3D v(sqrt(3.f) / 3.f, sqrt(3.f) / 3.f, sqrt(3.f) / 3.f);
  Vec3D vec = sphere->vectorFromObjectToWorldSpace(v);
  float eps{10E-5f};
  compareVectorsApprox(vec, Vec3D(0.2857f, 0.4286f, -0.8571f), eps);
}

// RED because the normal function of SceneElements has changed. TODO
/*TEST_F(WorldTest, findingNormalOfChildObjectOfTransformedWorld) {
  builder = make_unique<WorldBuilder>();
  builder->addLight(light);

  builder->createWorld();
  builder->applyWorldTransformation(rotationOverY(PI / 2.f));
  SceneElementPtr outerWorld = builder->getProduct();

  builder->createWorld();
  builder->applyWorldTransformation(scale(1.f, 2.f, 3.f));
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(5.f, 0.f, 0.f));
  builder->addElement();
  SceneElementPtr innerWorld = builder->getProduct();

  SceneElementRawPtr sphere = builder->getCurrentElement();
  outerWorld->add(innerWorld);

  EXPECT_EQ(sphere->getParent(), innerWorld.get());
  EXPECT_EQ(innerWorld->getParent(), outerWorld.get());
  EXPECT_EQ(outerWorld->getParent(), nullptr);

  Point3D point(1.7321f, 1.1547f, -5.5774f);
  Vec3D vec = sphere->normal(point);
  float eps{10E-5f};
  compareVectorsApprox(vec, Vec3D(0.2857f, 0.4286f, -0.8571f), eps);
}*/

/*TEST_F(Tworld, createsWorldOfOneNegativeIntersection) {
  Ray r = Ray(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  direct = make_shared<StandardSphere>();
  TraceablePtr s = direct->create(builder, prop);
  prop.setProperty(Props::SPHERE_CENTER, Point3D(0.0f, 0.0f, -8.0f))
      .setProperty(Props::COLOR, Vec3D(0.09f, 0.172f, 0.909f))
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
  Ray r = Ray(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  direct = make_shared<StandardSphere>();
  prop.setProperty(Props::SPHERE_CENTER, Point3D(0.0f, 0.0f, -11.0f))
      .setProperty(Props::COLOR, Vec3D(0.09f, 0.172f, 0.909f))
      .setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.f)
      .setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::SHININESS, 0.f);
  TraceablePtr s = direct->create(builder, prop);
  prop.setProperty(Props::SPHERE_CENTER, Point3D(0.0f, 0.0f, -8.0f));

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
  Ray r = Ray(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  prop.setProperty(Props::SPHERE_CENTER, Point3D(0.f, 0.f, 5.f))
      .setProperty(Props::COLOR, Vec3D(0.09f, 0.172f, 0.909f))
      .setProperty(Props::AMBIENT, 0.f)
      .setProperty(Props::DIFFUSE, 0.f)
      .setProperty(Props::SPECULAR, 0.f)
      .setProperty(Props::SHININESS, 0.f);

  direct = make_shared<StandardSphere>();
  TraceablePtr s = direct->create(builder, prop);

  prop.setProperty(Props::SPHERE_CENTER, Point3D(0.0f, 0.0f, 0.0f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s1 = direct2->create(builder, prop);

  prop.setProperty(Props::SPHERE_CENTER, Point3D(0.0f, 0.0f, -8.0f));
  SceneDirectorPtr direct3 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct3->create(builder, prop);

  prop.setProperty(Props::SPHERE_CENTER, Point3D(0.0f, 0.0f, 8.0f));
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
  prop.setProperty(Props::COLOR, Vec3D(0.8f, 1.0f, 0.6f))
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
  prop.setProperty(Props::COLOR, Vec3D(0.8f, 1.0f, 0.6f))
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

  Ray r(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  world->intersect(r);
  vector<float> v = w->intersectionsSorted();

  ASSERT_EQ(v.size(), 4);
  ASSERT_EQ(v[0], 4.0f);
  ASSERT_EQ(v[1], 4.5f);
  ASSERT_EQ(v[2], 5.5f);
  ASSERT_EQ(v[3], 6.0f);
}

TEST_F(Tworld, computesQuantitiesOfIntersection) {
  prop.setProperty(Props::COLOR, Vec3D(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);

  Ray r(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);

  ASSERT_TRUE(t->record().eye(r) == Vec3D(0.0f, 0.0f, -1.0f));
  ASSERT_TRUE(t->record().point(r) == Point3D(0.0f, 0.0f, -1.0f));
  ASSERT_TRUE(t->normal(t->record().point(r)) == Vec3D(0.0f, 0.0f, -1.0f));
}

TEST_F(Tworld, intersectionWhenHitOccursOutside) {
  prop.setProperty(Props::COLOR, Vec3D(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);

  Ray r(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  world->intersect(r);

  TraceablePtr t = world->closestHit(r);
  t->checkInside(r);

  ASSERT_FALSE(t->record().inside == true);
}

TEST_F(Tworld, intersectionWhenHitOccursInside) {
  prop.setProperty(Props::COLOR, Vec3D(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);

  Ray r(Point3D(0.0f, 0.0f, 0.0f), Vec3D(0.0f, 0.0f, 1.0f));
  world->intersect(r);

  TraceablePtr t = world->closestHit(r);
  t->checkInside(r);

  ASSERT_TRUE(t->record().inside == true);
}

TEST_F(Tworld, ShadingAnIntersection) {
  prop.setProperty(Props::COLOR, Vec3D(0.8f, 1.0f, 0.6f))
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

  Ray r(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  world->intersect(r);

  TraceablePtr t = world->closestHit(r);
  t->checkInside(r);

  PointLight l(Point3D(-10.0f, 10.0f, -10.0f), Vec3D(1.0f, 1.0f, 1.0f));
  t->setLight(l);
  Vec3D color = t->lighting(r);

  ASSERT_TRUE(t == s1);

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.38066f, eps);
  EXPECT_NEAR(color.y(), 0.47583f, eps);
  EXPECT_NEAR(color.z(), 0.2855f, eps);
}

TEST_F(Tworld, ShadingAnInsideIntersection) {
  prop.setProperty(Props::COLOR, Vec3D(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::COLOR, Vec3D(1.0f, 1.0f, 1.0f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.9f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::SHININESS, 200.f)
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);

  Ray r(Point3D(0.0f, 0.0f, 0.0f), Vec3D(0.0f, 0.0f, 1.0f));
  world->intersect(r);

  TraceablePtr t = world->closestHit(r);
  t->checkInside(r);

  PointLight l(Point3D(0.0f, 0.25f, 0.0f), Vec3D(1.0f, 1.0f, 1.0f));
  t->setLight(l);
  Vec3D color = t->lighting(r);

  ASSERT_TRUE(t == s2);

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.90498f, eps);
  EXPECT_NEAR(color.y(), 0.90498f, eps);
  EXPECT_NEAR(color.z(), 0.90498f, eps);
}

TEST_F(Tworld, colorWhenRayMisses) {
  prop.setProperty(Props::COLOR, Vec3D(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::COLOR, Vec3D(1.0f, 1.0f, 1.0f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.9f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::SHININESS, 200.f)
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);

  Ray r(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 1.0f, 0.0f));
  world->intersect(r);

  PointLight l(Point3D(0.0f, 0.00f, 0.0f), Vec3D(0.0f, 0.0f, 0.0f));
  world->setLight(l);
  TraceablePtr t = world->closestHit(r);
  ASSERT_FALSE(t);
}

TEST_F(Tworld, colorWhenRayHits) {
  prop.setProperty(Props::COLOR, Vec3D(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::COLOR, Vec3D(1.0f, 1.0f, 1.0f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.9f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::SHININESS, 200.f)
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);

  Ray r(Point3D(0.0f, 0.0f, -5.0f), Vec3D(0.0f, 0.0f, 1.0f));
  world->intersect(r);

  TraceablePtr t = world->closestHit(r);

  PointLight l(Point3D(-10.0f, 10.00f, -10.0f), Vec3D(1.0f, 1.0f, 1.0f));
  t->setLight(l);
  Vec3D color = t->lighting(r);

  float eps = 1E-3f;
  EXPECT_NEAR(color.x(), 0.38066f, eps);
  EXPECT_NEAR(color.y(), 0.47583f, eps);
  EXPECT_NEAR(color.z(), 0.2855f, eps);
}

TEST_F(Tworld, colorWithAnIntersectionBehind) {
  prop.setProperty(Props::COLOR, Vec3D(0.8f, 1.0f, 0.6f))
      .setProperty(Props::AMBIENT, 1.0f)
      .setProperty(Props::DIFFUSE, 0.7f)
      .setProperty(Props::SPECULAR, 0.2f);
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  prop.setProperty(Props::COLOR, Vec3D(1.0f, 1.0f, 1.0f))
      .setProperty(Props::AMBIENT, 0.1f)
      .setProperty(Props::DIFFUSE, 0.9f)
      .setProperty(Props::SPECULAR, 0.9f)
      .setProperty(Props::SHININESS, 200.f)
      .setProperty(Props::OBJECT_TRANSFROM_MATRIX, scale(0.5f, 0.5f, 0.5f));
  direct2 = make_shared<StandardSphere>();
  TraceablePtr s2 = direct2->create(builder, prop);
  world->add(s1);
  world->add(s2);

  Ray r(Point3D(0.0f, 0.0f, 0.75f), Vec3D(0.0f, 0.0f, -1.0f));
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);

  ASSERT_TRUE(t == s2);  // color of the inner sphere
}

TEST_F(Tworld, noShadowWhenNothingCollinear) {
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);
  Point3D p(0.0f, 10.0f, 0.0f);
  Ray r(p, (light.position() - p).normalize());
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);
  ASSERT_FALSE(t);  // No hit -> no shadow
}

TEST_F(Tworld, shadowWhenObjectBetweenLightAndPoint) {
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);
  Point3D p(10.0f, -10.0f, 10.0f);
  Ray r(p, (light.position() - p).normalize());
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);
  ASSERT_TRUE(t->isShadowed(p));
}

TEST_F(Tworld, noShadowWhenObjectBehindLight) {
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);
  Point3D p(-20.0f, 20.0f, -20.0f);
  Ray r(p, (light.position() - p).normalize());
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);
  ASSERT_FALSE(t->isShadowed(p));
}

TEST_F(Tworld, noShadowWhenObjectBehindPoint) {
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);
  Point3D p(-2.0f, 2.0f, -2.0f);
  Ray r(p, (light.position() - p).normalize());
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);
  ASSERT_FALSE(t);  // No hit -> now shadow
}

TEST_F(Tworld, intersectionWithShadows) {
  direct = make_shared<StandardSphere>();
  TraceablePtr s1 = direct->create(builder, prop);
  world->add(s1);
  prop.setProperty(Props::OBJECT_TRANSFROM_MATRIX, translation(0.0f,
0.0f, 10.0f)); direct2 = make_shared<StandardSphere>(); shared_ptr<SceneElement>
s2 = direct2->create(builder, prop); world->add(s2); Ray r(Point3D(0.0f,
0.0f, 5.0f), Vec3D(0.0f, 0.0f, 1.0f)); light = PointLight(Point3D(0.0f,
0.0f, 10.0f), Vec3D(1.0f, 1.0f, 1.0f)); world->setLight(light);
  world->intersect(r);
  TraceablePtr t = world->closestHit(r);
  t->setLight(light);
  Vec3D color = t->lighting(r);
  ASSERT_TRUE(color == Vec3D(0.1f, 0.1f, 0.1f));
}*/
