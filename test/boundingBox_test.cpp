#include "acceleration/bvh.h"
#include "composite/builder.h"
#include "composite/iterator.h"
#include "geometry/cylinder.h"
#include "geometry/sphere.h"
#include "gtesting.h"
#include "transformations/transformation.h"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::vector;

class BoundingBoxTest : public testing::RTest {
 public:
  BoundingBox box1;
  BoundingBox box2;
  MaterialProperties prop;
  IntersectionRecord rec;
  BVHierarchy bvh;
};

TEST_F(BoundingBoxTest, addsPointToBoundingBox) {
  box1.addPoint(Point3D(-5.f, 2.f, 0.f));
  box1.addPoint(Point3D(7.f, 0.f, -3.f));
  comparePoints(box1.minPoint(), Point3D(-5.f, 0.f, -3.f));
  comparePoints(box1.maxPoint(), Point3D(7.f, 2.f, 0.f));
}

TEST_F(BoundingBoxTest, addsBoxToBoundingBox) {
  box1 = BoundingBox(Point3D(-5.f, -2.f, 0.f), Point3D(7.f, 4.f, 4.f));
  box2 = BoundingBox(Point3D(8.f, -7.f, -2.f), Point3D(14.f, 2.f, 8.f));
  box1.addBox(box2);
  comparePoints(box1.minPoint(), Point3D(-5.f, -7.f, -2.f));
  comparePoints(box1.maxPoint(), Point3D(14.f, 4.f, 8.f));
}

TEST_F(BoundingBoxTest, checkIfBoxContainsPoint) {
  box1 = BoundingBox(Point3D(5.f, -2.f, 0.f), Point3D(11.f, 4.f, 7.f));
  EXPECT_TRUE(box1.containsPoint(Point3D(5.f, -2.f, 0.f)));
  EXPECT_TRUE(box1.containsPoint(Point3D(11.f, 4.f, 7.f)));
  EXPECT_TRUE(box1.containsPoint(Point3D(8.f, 1.f, 3.f)));
  EXPECT_FALSE(box1.containsPoint(Point3D(3.f, 0.f, 3.f)));
  EXPECT_FALSE(box1.containsPoint(Point3D(8.f, -4.f, 3.f)));
  EXPECT_FALSE(box1.containsPoint(Point3D(8.f, 1.f, -1.f)));
  EXPECT_FALSE(box1.containsPoint(Point3D(13.f, 1.f, 3.f)));
  EXPECT_FALSE(box1.containsPoint(Point3D(8.f, 5.f, 3.f)));
  EXPECT_FALSE(box1.containsPoint(Point3D(8.f, 1.f, 8.f)));
}

TEST_F(BoundingBoxTest, checkIfBoxContainsAnotherBox) {
  box1 = BoundingBox(Point3D(5.f, -2.f, 0.f), Point3D(11.f, 4.f, 7.f));
  box2 = BoundingBox(Point3D(5.f, -2.f, 0.f), Point3D(11.f, 4.f, 7.f));
  EXPECT_TRUE(box1.containsBoundingBox(box2));
  box2 = BoundingBox(Point3D(6.f, -1.f, 1.f), Point3D(10.f, 3.f, 6.f));
  EXPECT_TRUE(box1.containsBoundingBox(box2));
  box2 = BoundingBox(Point3D(4.f, -3.f, -1.f), Point3D(10.f, 3.f, 6.f));
  EXPECT_FALSE(box1.containsBoundingBox(box2));
  box2 = BoundingBox(Point3D(6.f, -1.f, 1.f), Point3D(12.f, 5.f, 8.f));
  EXPECT_FALSE(box1.containsBoundingBox(box2));
}

TEST_F(BoundingBoxTest, transformsBoundingBox) {
  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->processSceneElement(new Sphere);
  builder->createBBoxForElement(
      BoundingBox(Point3D(-1.f, -1.f, -1.f), Point3D(1.f, 1.f, 1.f)));
  builder->applyTransformation(rotationOverX(PI / 4.f) *
                               rotationOverY(PI / 4.f));
  BoundingBox p = builder->getCurrentElement()->boundingBox();

  float eps = 1E-4f;
  comparePointsApprox(p.minPoint(), Point3D(-1.4142f, -1.7071f, -1.7071f), eps);
  comparePointsApprox(p.maxPoint(), Point3D(1.4142f, 1.7071f, 1.7071f), eps);
}

TEST_F(BoundingBoxTest, boundsOfSceneElementInParentSpace) {
  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(Vec3D(1.f, -3.f, 5.f)) *
                               scale(Vec3D(0.5f, 2.f, 4.f)));
  BoundingBox p = builder->getCurrentElement()->boundingBox();

  float eps = 1E-4f;
  comparePointsApprox(p.minPoint(), Point3D(0.5f, -5.f, 1.f), eps);
  comparePointsApprox(p.maxPoint(), Point3D(1.5f, -1.f, 9.f), eps);
}

TEST_F(BoundingBoxTest, boundingBoxOfWorld) {
  BuilderPtr builder = std::make_unique<WorldBuilder>();
  builder->createWorld();
  builder->processSceneElement(new Sphere);
  builder->applyTransformation(translation(Vec3D(2.f, 5.f, -3.f)) *
                               scale(Vec3D(2.f, 2.f, 2.f)));
  builder->addElement();
  builder->processSceneElement(new Cylinder(-2.f, 2.f, true));
  builder->applyTransformation(translation(Vec3D(-4.f, -1.f, 4.f)) *
                               scale(Vec3D(0.5f, 1.f, 0.5f)));
  builder->addElement();
  BoundingBox p = builder->getProduct()->boundingBox();

  float eps = 1E-4f;
  comparePointsApprox(p.minPoint(), Point3D(-4.5f, -3.f, -5.f), eps);
  comparePointsApprox(p.maxPoint(), Point3D(4.f, 7.f, 4.5f), eps);
}

TEST_F(BoundingBoxTest, intersectRayWithBBox) {
  box1 = BoundingBox(Point3D(-1.f, -1.f, -1.f), Point3D(1.f, 1.f, 1.f));
  Ray r(Point3D(5.f, 0.5f, 0.f), getUnitVectorOf(Vec3D(-1.f, 0.f, 0.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(-5.f, 0.5f, 0.f), getUnitVectorOf(Vec3D(1.f, 0.f, 0.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(0.5f, 5.f, 0.f), getUnitVectorOf(Vec3D(0.f, -1.f, 0.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(0.5f, -5.f, 0.f), getUnitVectorOf(Vec3D(0.f, 1.f, 0.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(0.5f, 0.f, 5.f), getUnitVectorOf(Vec3D(0.f, 0.f, -1.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(0.5f, 0.f, -5.f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(0.f, 0.5f, 0.f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(-2.f, 0.f, 0.f), getUnitVectorOf(Vec3D(2.f, 4.f, 6.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
  r = Ray(Point3D(0.f, -2.f, 0.f), getUnitVectorOf(Vec3D(6.f, 2.f, 4.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
  r = Ray(Point3D(0.f, 0.f, -2.f), getUnitVectorOf(Vec3D(4.f, 6.f, 2.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
  r = Ray(Point3D(2.f, 0.f, 2.f), getUnitVectorOf(Vec3D(0.f, 0.f, -1.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
  r = Ray(Point3D(0.f, 2.f, 2.f), getUnitVectorOf(Vec3D(0.f, -1.f, 0.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
  r = Ray(Point3D(2.f, 2.f, 0.f), getUnitVectorOf(Vec3D(-1.f, 0.f, 0.f)));
  EXPECT_FALSE(box1.intersectsRay(r));

  box1 = BoundingBox(Point3D(5.f, -2.f, 0.f), Point3D(11.f, 4.f, 7.f));
  r = Ray(Point3D(15.f, 1.f, 2.f), getUnitVectorOf(Vec3D(-1.f, 0.f, 0.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(-5.f, -1.f, 4.f), getUnitVectorOf(Vec3D(1.f, 0.f, 0.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(7.f, 6.f, 5.f), getUnitVectorOf(Vec3D(0.f, -1.f, 0.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(9.f, -5.f, 6.f), getUnitVectorOf(Vec3D(0.f, 1.f, 0.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(8.f, 2.f, 12.f), getUnitVectorOf(Vec3D(0.f, 0.f, -1.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(6.f, 0.f, -5.f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(8.f, 1.f, 3.5f), getUnitVectorOf(Vec3D(0.f, 0.f, 1.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  r = Ray(Point3D(9.f, -1.f, -8.f), getUnitVectorOf(Vec3D(2.f, 4.f, 6.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
  r = Ray(Point3D(8.f, 3.f, -4.f), getUnitVectorOf(Vec3D(6.f, 2.f, 4.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
  r = Ray(Point3D(9.f, -1.f, -2.f), getUnitVectorOf(Vec3D(4.f, 6.f, 2.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
  r = Ray(Point3D(4.f, 0.f, 9.f), getUnitVectorOf(Vec3D(0.f, 0.f, -1.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
  r = Ray(Point3D(8.f, 6.f, -1.f), getUnitVectorOf(Vec3D(0.f, -1.f, 0.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
  r = Ray(Point3D(12.f, 5.f, 4.f), getUnitVectorOf(Vec3D(-1.f, 0.f, 0.f)));
  EXPECT_FALSE(box1.intersectsRay(r));
}

TEST_F(BoundingBoxTest, splitBox) {
  box1 = BoundingBox(Point3D(-1.f, -4.f, -5.f), Point3D(9.f, 6.f, 5.f));
  BoundingBoxPair splittedBox = bvh.splitBoundsOf(box1);
  comparePoints(splittedBox.first.minPoint(), Point3D(-1.f, -4.f, -5.f));
  comparePoints(splittedBox.first.maxPoint(), Point3D(4.f, 6.f, 5.f));
  comparePoints(splittedBox.second.minPoint(), Point3D(4.f, -4.f, -5.f));
  comparePoints(splittedBox.second.maxPoint(), Point3D(9.f, 6.f, 5.f));

  box1 = BoundingBox(Point3D(-1.f, -2.f, -3.f), Point3D(9.f, 5.5f, 3.f));
  splittedBox = bvh.splitBoundsOf(box1);
  comparePoints(splittedBox.first.minPoint(), Point3D(-1.f, -2.f, -3.f));
  comparePoints(splittedBox.first.maxPoint(), Point3D(4.f, 5.5f, 3.f));
  comparePoints(splittedBox.second.minPoint(), Point3D(4.f, -2.f, -3.f));
  comparePoints(splittedBox.second.maxPoint(), Point3D(9.f, 5.5f, 3.f));

  box1 = BoundingBox(Point3D(-1.f, -2.f, -3.f), Point3D(5.f, 8.f, 3.f));
  splittedBox = bvh.splitBoundsOf(box1);
  comparePoints(splittedBox.first.minPoint(), Point3D(-1.f, -2.f, -3.f));
  comparePoints(splittedBox.first.maxPoint(), Point3D(5.f, 3.f, 3.f));
  comparePoints(splittedBox.second.minPoint(), Point3D(-1.f, 3.f, -3.f));
  comparePoints(splittedBox.second.maxPoint(), Point3D(5.f, 8.f, 3.f));

  box1 = BoundingBox(Point3D(-1.f, -2.f, -3.f), Point3D(5.f, 3.f, 7.f));
  splittedBox = bvh.splitBoundsOf(box1);
  comparePoints(splittedBox.first.minPoint(), Point3D(-1.f, -2.f, -3.f));
  comparePoints(splittedBox.first.maxPoint(), Point3D(5.f, 3.f, 2.f));
  comparePoints(splittedBox.second.minPoint(), Point3D(-1.f, -2.f, 2.f));
  comparePoints(splittedBox.second.maxPoint(), Point3D(5.f, 3.f, 7.f));
}

TEST_F(BoundingBoxTest, splitChildrenOfWorld) {
  WorldBuilder builder;
  builder.createWorld();

  builder.processSceneElement(new Sphere);
  builder.applyTransformation(translation(-2.f, 0.f, 0.f));
  SceneElementRawPtr sphere1 = builder.getCurrentElement();
  builder.addElement();

  builder.processSceneElement(new Sphere);
  builder.applyTransformation(translation(2.f, 0.f, 0.f));
  SceneElementRawPtr sphere2 = builder.getCurrentElement();
  builder.addElement();

  builder.processSceneElement(new Sphere);
  SceneElementRawPtr sphere3 = builder.getCurrentElement();
  builder.addElement();

  SceneElementPtr world = builder.getProduct();
  WorldPair wp =
      bvh.splitElementsOf(world->getChildren(), world->boundingBox());

  ASSERT_TRUE(world->getChildren().size() == 1);
  ASSERT_TRUE((*world->getChildren().begin()).get() == sphere3);

  ASSERT_TRUE(wp.first->getChildren().size() == 1);
  ASSERT_TRUE((*wp.first->getChildren().begin()).get() == sphere1);

  ASSERT_TRUE(wp.second->getChildren().size() == 1);
  ASSERT_TRUE((*wp.second->getChildren().begin()).get() == sphere2);
}

TEST_F(BoundingBoxTest, divideWorld) {
  WorldBuilder builder;
  builder.createWorld();

  builder.processSceneElement(new Sphere);
  builder.applyTransformation(translation(-2.f, -2.f, 0.f));
  SceneElementRawPtr sphere1 = builder.getCurrentElement();
  builder.addElement();

  builder.processSceneElement(new Sphere);
  builder.applyTransformation(translation(-2.f, 2.f, 0.f));
  SceneElementRawPtr sphere2 = builder.getCurrentElement();
  builder.addElement();

  builder.processSceneElement(new Sphere);
  builder.applyTransformation(scale(4.f, 4.f, 4.f));
  SceneElementRawPtr sphere3 = builder.getCurrentElement();
  builder.addElement();

  SceneElementPtr world = builder.getProduct();
  bvh.divideWorld(world, 1);
  WorldIterator it(world->getChildren());
  if (it.first()) {
    ASSERT_TRUE(it.currentElement() == sphere3);
    ASSERT_FALSE(it.currentElement()->isWorld());
    it.advance();
    ASSERT_TRUE(it.currentElement()->isWorld());
    ASSERT_TRUE(it.currentElement()->getChildren().size() == 2);
    WorldIterator it1(it.currentElement()->getChildren());
    if (it1.first()) {
      ASSERT_TRUE(it1.currentElement()->isWorld());
      ASSERT_TRUE(it1.currentElement()->getChildren().size() == 1);
      ASSERT_TRUE((*it1.currentElement()->getChildren().begin()).get() ==
                  sphere1);
      it1.advance();
      ASSERT_TRUE(it1.currentElement()->isWorld());
      ASSERT_TRUE(it1.currentElement()->getChildren().size() == 1);
      ASSERT_TRUE((*it1.currentElement()->getChildren().begin()).get() ==
                  sphere2);
    }
  }
}
