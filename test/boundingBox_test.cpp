#include "acceleration/scene_element_proxy.h"
#include "composite/builder.h"
#include "geometry/sphere.h"
#include "gtest/gtest.h"
#include "transformations/transformer.h"

using namespace testing;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::vector;

class BoundingBoxTest : public Test {
 public:
  SceneElementPtr box1;
  SceneElementPtr box2;
  MaterialProperties prop;
  IntersectionRecord rec;
};

TEST_F(BoundingBoxTest, addsPointToBoundingBox) {
  box1 = std::make_shared<SceneElementProxy>(nullptr);
  box1->addPoint(Point3f(-5.f, 2.f, 0.f));
  box1->addPoint(Point3f(7.f, 0.f, -3.f));
  BoundingBoxProperties p = box1->boundingBoxProperties();

  EXPECT_FLOAT_EQ(p.minPoint().x(), -5.f);
  EXPECT_FLOAT_EQ(p.minPoint().y(), 0.f);
  EXPECT_FLOAT_EQ(p.minPoint().z(), -3.f);

  EXPECT_FLOAT_EQ(p.maxPoint().x(), 7.f);
  EXPECT_FLOAT_EQ(p.maxPoint().y(), 2.f);
  EXPECT_FLOAT_EQ(p.maxPoint().z(), 0.f);
}

TEST_F(BoundingBoxTest, addsBoxToBoundingBox) {
  BuilderPtr builder = make_unique<WorldBuilder>();
  builder->createWorld(PointLight());
  SceneElementPtr world = builder->getProduct();
  SceneElementPtr sphere = std::make_shared<Sphere>();

  box1 = std::make_shared<SceneElementProxy>(world, Point3f(-5.f, -2.f, 0.f),
                                             Point3f(7.f, 4.f, 4.f));
  box2 = std::make_shared<SceneElementProxy>(sphere, Point3f(8.f, -7.f, -2.f),
                                             Point3f(14.f, 2.f, 8.f));
  box1->add(box2);
  BoundingBoxProperties p = box1->boundingBoxProperties();

  EXPECT_FLOAT_EQ(p.minPoint().x(), -5.f);
  EXPECT_FLOAT_EQ(p.minPoint().y(), -7.f);
  EXPECT_FLOAT_EQ(p.minPoint().z(), -2.f);

  EXPECT_FLOAT_EQ(p.maxPoint().x(), 14.f);
  EXPECT_FLOAT_EQ(p.maxPoint().y(), 4.f);
  EXPECT_FLOAT_EQ(p.maxPoint().z(), 8.f);
}

TEST_F(BoundingBoxTest, checkIfBoxContainsPoint) {
  box1 = std::make_shared<SceneElementProxy>(nullptr, Point3f(5.f, -2.f, 0.f),
                                             Point3f(11.f, 4.f, 7.f));
  EXPECT_TRUE(box1->containsPoint(Point3f(5.f, -2.f, 0.f)));
  EXPECT_TRUE(box1->containsPoint(Point3f(11.f, 4.f, 7.f)));
  EXPECT_TRUE(box1->containsPoint(Point3f(8.f, 1.f, 3.f)));
  EXPECT_FALSE(box1->containsPoint(Point3f(3.f, 0.f, 3.f)));
  EXPECT_FALSE(box1->containsPoint(Point3f(8.f, -4.f, 3.f)));
  EXPECT_FALSE(box1->containsPoint(Point3f(8.f, 1.f, -1.f)));
  EXPECT_FALSE(box1->containsPoint(Point3f(13.f, 1.f, 3.f)));
  EXPECT_FALSE(box1->containsPoint(Point3f(8.f, 5.f, 3.f)));
  EXPECT_FALSE(box1->containsPoint(Point3f(8.f, 1.f, 8.f)));
}

TEST_F(BoundingBoxTest, checkIfBoxContainsAnotherBox) {
  box1 = std::make_shared<SceneElementProxy>(nullptr, Point3f(5.f, -2.f, 0.f),
                                             Point3f(11.f, 4.f, 7.f));
  box2 = std::make_shared<SceneElementProxy>(nullptr, Point3f(5.f, -2.f, 0.f),
                                             Point3f(11.f, 4.f, 7.f));
  EXPECT_TRUE(box1->containsBoundingBox(box2->boundingBoxProperties()));
  box2->boundingBoxProperties() =
      BoundingBoxProperties(Point3f(6.f, -1.f, 1.f), Point3f(10.f, 3.f, 6.f));
  EXPECT_TRUE(box1->containsBoundingBox(box2->boundingBoxProperties()));
  box2->boundingBoxProperties() =
      BoundingBoxProperties(Point3f(4.f, -3.f, -1.f), Point3f(10.f, 3.f, 6.f));
  EXPECT_FALSE(box1->containsBoundingBox(box2->boundingBoxProperties()));
  box2->boundingBoxProperties() =
      BoundingBoxProperties(Point3f(6.f, -1.f, 1.f), Point3f(12.f, 5.f, 8.f));
  EXPECT_FALSE(box1->containsBoundingBox(box2->boundingBoxProperties()));
}

TEST_F(BoundingBoxTest, transformsBoundingBox) {
  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(rotationOverX(PI / 4.f) *
                               rotationOverY(PI / 4.f));
  SceneElementPtr sphere(builder->getCurrentElement());
  SceneElementPtr box = std::make_shared<SceneElementProxy>(
      sphere, Point3f(-1.f, -1.f, -1.f), Point3f(1.f, 1.f, 1.f));
  BoundingBoxProperties p = box->boundingBoxProperties();

  float eps = 1E-4f;
  EXPECT_NEAR(p.minPoint().x(), -1.4142f, eps);
  EXPECT_NEAR(p.minPoint().y(), -1.7071f, eps);
  EXPECT_NEAR(p.minPoint().z(), -1.7071f, eps);

  EXPECT_NEAR(p.maxPoint().x(), 1.4142f, eps);
  EXPECT_NEAR(p.maxPoint().y(), 1.7071f, eps);
  EXPECT_NEAR(p.maxPoint().z(), 1.7071f, eps);
}
