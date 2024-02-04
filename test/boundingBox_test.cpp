#include "composite/builder.h"
#include "geometry/cylinder.h"
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
  BoundingBox box1;
  BoundingBox box2;
  MaterialProperties prop;
  IntersectionRecord rec;
};

TEST_F(BoundingBoxTest, addsPointToBoundingBox) {
  box1.addPoint(Point3f(-5.f, 2.f, 0.f));
  box1.addPoint(Point3f(7.f, 0.f, -3.f));

  EXPECT_FLOAT_EQ(box1.minPoint().x(), -5.f);
  EXPECT_FLOAT_EQ(box1.minPoint().y(), 0.f);
  EXPECT_FLOAT_EQ(box1.minPoint().z(), -3.f);

  EXPECT_FLOAT_EQ(box1.maxPoint().x(), 7.f);
  EXPECT_FLOAT_EQ(box1.maxPoint().y(), 2.f);
  EXPECT_FLOAT_EQ(box1.maxPoint().z(), 0.f);
}

TEST_F(BoundingBoxTest, addsBoxToBoundingBox) {
  box1 = BoundingBox(Point3f(-5.f, -2.f, 0.f), Point3f(7.f, 4.f, 4.f));
  box2 = BoundingBox(Point3f(8.f, -7.f, -2.f), Point3f(14.f, 2.f, 8.f));
  box1.addBox(box2);

  EXPECT_FLOAT_EQ(box1.minPoint().x(), -5.f);
  EXPECT_FLOAT_EQ(box1.minPoint().y(), -7.f);
  EXPECT_FLOAT_EQ(box1.minPoint().z(), -2.f);

  EXPECT_FLOAT_EQ(box1.maxPoint().x(), 14.f);
  EXPECT_FLOAT_EQ(box1.maxPoint().y(), 4.f);
  EXPECT_FLOAT_EQ(box1.maxPoint().z(), 8.f);
}

TEST_F(BoundingBoxTest, checkIfBoxContainsPoint) {
  box1 = BoundingBox(Point3f(5.f, -2.f, 0.f), Point3f(11.f, 4.f, 7.f));
  EXPECT_TRUE(box1.containsPoint(Point3f(5.f, -2.f, 0.f)));
  EXPECT_TRUE(box1.containsPoint(Point3f(11.f, 4.f, 7.f)));
  EXPECT_TRUE(box1.containsPoint(Point3f(8.f, 1.f, 3.f)));
  EXPECT_FALSE(box1.containsPoint(Point3f(3.f, 0.f, 3.f)));
  EXPECT_FALSE(box1.containsPoint(Point3f(8.f, -4.f, 3.f)));
  EXPECT_FALSE(box1.containsPoint(Point3f(8.f, 1.f, -1.f)));
  EXPECT_FALSE(box1.containsPoint(Point3f(13.f, 1.f, 3.f)));
  EXPECT_FALSE(box1.containsPoint(Point3f(8.f, 5.f, 3.f)));
  EXPECT_FALSE(box1.containsPoint(Point3f(8.f, 1.f, 8.f)));
}

TEST_F(BoundingBoxTest, checkIfBoxContainsAnotherBox) {
  box1 = BoundingBox(Point3f(5.f, -2.f, 0.f), Point3f(11.f, 4.f, 7.f));
  box2 = BoundingBox(Point3f(5.f, -2.f, 0.f), Point3f(11.f, 4.f, 7.f));
  EXPECT_TRUE(box1.containsBoundingBox(box2));
  box2 = BoundingBox(Point3f(6.f, -1.f, 1.f), Point3f(10.f, 3.f, 6.f));
  EXPECT_TRUE(box1.containsBoundingBox(box2));
  box2 = BoundingBox(Point3f(4.f, -3.f, -1.f), Point3f(10.f, 3.f, 6.f));
  EXPECT_FALSE(box1.containsBoundingBox(box2));
  box2 = BoundingBox(Point3f(6.f, -1.f, 1.f), Point3f(12.f, 5.f, 8.f));
  EXPECT_FALSE(box1.containsBoundingBox(box2));
}

TEST_F(BoundingBoxTest, transformsBoundingBox) {
  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->processSceneElement(new Sphere);
  builder->createBBoxForElement(
      BoundingBox(Point3f(-1.f, -1.f, -1.f), Point3f(1.f, 1.f, 1.f)));
  builder->applyTransformation(rotationOverX(PI / 4.f) *
                               rotationOverY(PI / 4.f));
  BoundingBox p = builder->getCurrentElement()->boundingBox();

  float eps = 1E-4f;
  EXPECT_NEAR(p.minPoint().x(), -1.4142f, eps);
  EXPECT_NEAR(p.minPoint().y(), -1.7071f, eps);
  EXPECT_NEAR(p.minPoint().z(), -1.7071f, eps);

  EXPECT_NEAR(p.maxPoint().x(), 1.4142f, eps);
  EXPECT_NEAR(p.maxPoint().y(), 1.7071f, eps);
  EXPECT_NEAR(p.maxPoint().z(), 1.7071f, eps);
}

TEST_F(BoundingBoxTest, boundsOfSceneElementInParentSpace) {
  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(Vec3f(1.f, -3.f, 5.f)) *
                               scale(Vec3f(0.5f, 2.f, 4.f)));
  BoundingBox p = builder->getCurrentElement()->boundingBox();

  float eps = 1E-4f;
  EXPECT_NEAR(p.minPoint().x(), 0.5f, eps);
  EXPECT_NEAR(p.minPoint().y(), -5.f, eps);
  EXPECT_NEAR(p.minPoint().z(), 1.f, eps);

  EXPECT_NEAR(p.maxPoint().x(), 1.5f, eps);
  EXPECT_NEAR(p.maxPoint().y(), -1.f, eps);
  EXPECT_NEAR(p.maxPoint().z(), 9.f, eps);
}

/*TEST_F(BoundingBoxTest, boundingBoxOfWorld) {
  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->createWorld(PointLight());
  builder->addWorldToBoundingbox();
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(Vec3f(2.f, 5.f, -3.f)) *
                               scale(Vec3f(2.f, 2.f, 2.f)));
  builder->addElement();
  builder->processSceneElement(new Cylinder(-2.f, 2.f, true));
  builder->applyTransformation(translation(Vec3f(-4.f, -1.f, 4.f)) *
                               scale(Vec3f(0.5f, 1.f, 0.5f)));
  builder->addElement();
  BoundingBox p = builder->getProductBBox()->boundingBoxProperties();

  float eps = 1E-4f;
  EXPECT_NEAR(p.minPoint().x(), -4.5f, eps);
  EXPECT_NEAR(p.minPoint().y(), -3.f, eps);
  EXPECT_NEAR(p.minPoint().z(), -5.f, eps);

  EXPECT_NEAR(p.maxPoint().x(), 4.5f, eps);
  EXPECT_NEAR(p.maxPoint().y(), 7.f, eps);
  EXPECT_NEAR(p.maxPoint().z(), 4.5f, eps);
}*/
