#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mat3.h>
#include <vec3.h>

#include "acceleration/bvh.h"
#include "application/parameters.h"
#include "composite/builder.h"
#include "composite/iterator.h"
#include "geometry/cylinder.h"
#include "geometry/sphere.h"
#include "transformations/transformation.h"
#include "utils.h"

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::vector;
using app = AppParameters;
using testing::Eq;

class BoundingBoxTest : public testing::Test {
 public:
  Vec3f white = Vec3f(1.f, 1.f, 1.f);
  BoundingBox box1;
  BoundingBox box2;
  DataContainer prop;
  Intersection rec;
  BVHierarchy bvh;
  float eps = 1.E-4f;
};

TEST_F(BoundingBoxTest, addsPointToBoundingBox) {
  box1.addPoint(Point3f(-5.f, 2.f, 0.f));
  box1.addPoint(Point3f(7.f, 0.f, -3.f));
  EXPECT_THAT(box1.minPoint(), Eq(Point3f(-5.f, 0.f, -3.f)));
  EXPECT_THAT(box1.maxPoint(), Eq(Point3f(7.f, 2.f, 0.f)));
}

TEST_F(BoundingBoxTest, addsBoxToBoundingBox) {
  box1 = BoundingBox(Point3f(-5.f, -2.f, 0.f), Point3f(7.f, 4.f, 4.f));
  box2 = BoundingBox(Point3f(8.f, -7.f, -2.f), Point3f(14.f, 2.f, 8.f));
  box1.addBox(box2);
  EXPECT_THAT(box1.minPoint(), Eq(Point3f(-5.f, -7.f, -2.f)));
  EXPECT_THAT(box1.maxPoint(), Eq(Point3f(14.f, 4.f, 8.f)));
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
  auto box3 = BoundingBox(Point3f(6.f, -1.f, 1.f), Point3f(10.f, 3.f, 6.f));
  auto box4 = BoundingBox(Point3f(4.f, -3.f, -1.f), Point3f(10.f, 3.f, 6.f));
  auto box5 = BoundingBox(Point3f(6.f, -1.f, 1.f), Point3f(12.f, 5.f, 8.f));
  EXPECT_TRUE(box1.containsBoundingBox(box2));
  EXPECT_TRUE(box1.containsBoundingBox(box3));
  EXPECT_FALSE(box1.containsBoundingBox(box4));
  EXPECT_FALSE(box1.containsBoundingBox(box5));
}

TEST_F(BoundingBoxTest, transformsBoundingBox) {
  /*PrimitiveBuilder builder;
  builder.buildPrimitive();
  builder->createBBoxForElement(
      BoundingBox(Point3f(-1.f, -1.f, -1.f), Point3f(1.f, 1.f, 1.f)));
  builder->applyTransformation(rotationOverX(PI / 4.f) *
                               rotationOverY(PI / 4.f));
  BoundingBox p = builder->getCurrentElement()->boundingBox();

  EXPECT_THAT(p.minPoint(), Point3f(-1.4142f, -1.7071f, -1.7071f), eps);
  EXPECT_THAT(p.maxPoint(), Point3f(1.4142f, 1.7071f, 1.7071f), eps);*/
}

TEST_F(BoundingBoxTest, boundsOfSceneElementInParentSpace) {
  EntityFactory builder;
  SceneDirector director;
  DataContainer data;
  data.setProperty(app::PRIMITIVE_TYPE, app::SPHERE)
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN)
      .setProperty(app::COLOR, white)
      .setProperty(
          app::TRANSFORMATION_MATRIX,
          translation(Vec3f(1.f, -3.f, 5.f)) * scale(Vec3f(0.5f, 2.f, 4.f)));
  director.createSceneElement(builder, data);
  auto p = director.getCurrentElement()->getBounds();

  EXPECT_THAT(p.minPoint(), Point3Near(Point3f(0.5f, -5.f, 1.f), eps));
  EXPECT_THAT(p.maxPoint(), Point3Near(Point3f(1.5f, -1.f, 9.f), eps));
}

TEST_F(BoundingBoxTest, boundingBoxOfWorld) {
  WorldBuilder world_builder;
  EntityFactory primitive_builder;
  SceneDirector scene_director;

  DataContainer data;
  data.setProperty(app::PRIMITIVE_TYPE, app::SPHERE)
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN)
      .setProperty(app::COLOR, white)
      .setProperty(
          app::TRANSFORMATION_MATRIX,
          translation(Vec3f(2.f, 5.f, -3.f)) * scale(Vec3f(2.f, 2.f, 2.f)));
  scene_director.createSceneElement(primitive_builder, data);

  data.setProperty(app::PRIMITIVE_TYPE, app::CYLINDER)
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN)
      .setProperty(app::COLOR, white)
      .setProperty(
          app::TRANSFORMATION_MATRIX,
          translation(Vec3f(-4.f, -1.f, 4.f)) * scale(Vec3f(0.5f, 1.f, 0.5f)))
      .setProperty(app::CYLINDER_Y_MIN, -2.f)
      .setProperty(app::CYLINDER_Y_MAX, 2.f)
      .setProperty(app::CYLINDER_CLOSED, true);
  scene_director.createSceneElement(primitive_builder, data);

  scene_director.createWorld(world_builder, PointLight());

  BoundingBox p = scene_director.getSceneProduct()->getBounds();

  EXPECT_THAT(p.minPoint(), Point3Near(Point3f(-4.5f, -3.f, -5.f), eps));
  EXPECT_THAT(p.maxPoint(), Point3Near(Point3f(4.f, 7.f, 4.5f), eps));
}

TEST_F(BoundingBoxTest, intersectRayWithBBox) {
  box1 = BoundingBox(Point3f(-1.f, -1.f, -1.f), Point3f(1.f, 1.f, 1.f));
  Ray r(Point3f(5.f, 0.5f, 0.f), normalized(Vec3f(-1.f, 0.f, 0.f)));
  auto r1 = Ray(Point3f(-5.f, 0.5f, 0.f), normalized(Vec3f(1.f, 0.f, 0.f)));
  auto r2 = Ray(Point3f(0.5f, 5.f, 0.f), normalized(Vec3f(0.f, -1.f, 0.f)));
  auto r3 = Ray(Point3f(0.5f, -5.f, 0.f), normalized(Vec3f(0.f, 1.f, 0.f)));
  auto r4 = Ray(Point3f(0.5f, 0.f, 5.f), normalized(Vec3f(0.f, 0.f, -1.f)));
  auto r5 = Ray(Point3f(0.5f, 0.f, -5.f), normalized(Vec3f(0.f, 0.f, 1.f)));
  auto r6 = Ray(Point3f(0.f, 0.5f, 0.f), normalized(Vec3f(0.f, 0.f, 1.f)));
  auto r7 = Ray(Point3f(-2.f, 0.f, 0.f), normalized(Vec3f(2.f, 4.f, 6.f)));
  auto r8 = Ray(Point3f(0.f, -2.f, 0.f), normalized(Vec3f(6.f, 2.f, 4.f)));
  auto r9 = Ray(Point3f(0.f, 0.f, -2.f), normalized(Vec3f(4.f, 6.f, 2.f)));
  auto r10 = Ray(Point3f(2.f, 0.f, 2.f), normalized(Vec3f(0.f, 0.f, -1.f)));
  auto r11 = Ray(Point3f(0.f, 2.f, 2.f), normalized(Vec3f(0.f, -1.f, 0.f)));
  auto r12 = Ray(Point3f(2.f, 2.f, 0.f), normalized(Vec3f(-1.f, 0.f, 0.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  EXPECT_TRUE(box1.intersectsRay(r1));
  EXPECT_TRUE(box1.intersectsRay(r2));
  EXPECT_TRUE(box1.intersectsRay(r3));
  EXPECT_TRUE(box1.intersectsRay(r4));
  EXPECT_TRUE(box1.intersectsRay(r5));
  EXPECT_TRUE(box1.intersectsRay(r6));
  EXPECT_FALSE(box1.intersectsRay(r7));
  EXPECT_FALSE(box1.intersectsRay(r8));
  EXPECT_FALSE(box1.intersectsRay(r9));
  EXPECT_FALSE(box1.intersectsRay(r10));
  EXPECT_FALSE(box1.intersectsRay(r11));
  EXPECT_FALSE(box1.intersectsRay(r12));

  box1 = BoundingBox(Point3f(5.f, -2.f, 0.f), Point3f(11.f, 4.f, 7.f));
  r = Ray(Point3f(15.f, 1.f, 2.f), normalized(Vec3f(-1.f, 0.f, 0.f)));
  r1 = Ray(Point3f(-5.f, -1.f, 4.f), normalized(Vec3f(1.f, 0.f, 0.f)));
  r2 = Ray(Point3f(7.f, 6.f, 5.f), normalized(Vec3f(0.f, -1.f, 0.f)));
  r3 = Ray(Point3f(9.f, -5.f, 6.f), normalized(Vec3f(0.f, 1.f, 0.f)));
  r4 = Ray(Point3f(8.f, 2.f, 12.f), normalized(Vec3f(0.f, 0.f, -1.f)));
  r5 = Ray(Point3f(6.f, 0.f, -5.f), normalized(Vec3f(0.f, 0.f, 1.f)));
  r6 = Ray(Point3f(8.f, 1.f, 3.5f), normalized(Vec3f(0.f, 0.f, 1.f)));
  r7 = Ray(Point3f(9.f, -1.f, -8.f), normalized(Vec3f(2.f, 4.f, 6.f)));
  r8 = Ray(Point3f(8.f, 3.f, -4.f), normalized(Vec3f(6.f, 2.f, 4.f)));
  r9 = Ray(Point3f(9.f, -1.f, -2.f), normalized(Vec3f(4.f, 6.f, 2.f)));
  r10 = Ray(Point3f(4.f, 0.f, 9.f), normalized(Vec3f(0.f, 0.f, -1.f)));
  r11 = Ray(Point3f(8.f, 6.f, -1.f), normalized(Vec3f(0.f, -1.f, 0.f)));
  r12 = Ray(Point3f(12.f, 5.f, 4.f), normalized(Vec3f(-1.f, 0.f, 0.f)));
  EXPECT_TRUE(box1.intersectsRay(r));
  EXPECT_TRUE(box1.intersectsRay(r1));
  EXPECT_TRUE(box1.intersectsRay(r2));
  EXPECT_TRUE(box1.intersectsRay(r3));
  EXPECT_TRUE(box1.intersectsRay(r4));
  EXPECT_TRUE(box1.intersectsRay(r5));
  EXPECT_TRUE(box1.intersectsRay(r6));
  EXPECT_FALSE(box1.intersectsRay(r7));
  EXPECT_FALSE(box1.intersectsRay(r8));
  EXPECT_FALSE(box1.intersectsRay(r9));
  EXPECT_FALSE(box1.intersectsRay(r10));
  EXPECT_FALSE(box1.intersectsRay(r11));
  EXPECT_FALSE(box1.intersectsRay(r12));
}

TEST_F(BoundingBoxTest, splitBox) {
  box1 = BoundingBox(Point3f(-1.f, -4.f, -5.f), Point3f(9.f, 6.f, 5.f));
  BoundingBoxPair splittedBox = bvh.splitBoundsOf(box1);
  EXPECT_THAT(splittedBox.first.minPoint(), Eq(Point3f(-1.f, -4.f, -5.f)));
  EXPECT_THAT(splittedBox.first.maxPoint(), Eq(Point3f(4.f, 6.f, 5.f)));
  EXPECT_THAT(splittedBox.second.minPoint(), Eq(Point3f(4.f, -4.f, -5.f)));
  EXPECT_THAT(splittedBox.second.maxPoint(), Eq(Point3f(9.f, 6.f, 5.f)));

  box1 = BoundingBox(Point3f(-1.f, -2.f, -3.f), Point3f(9.f, 5.5f, 3.f));
  splittedBox = bvh.splitBoundsOf(box1);
  EXPECT_THAT(splittedBox.first.minPoint(), Eq(Point3f(-1.f, -2.f, -3.f)));
  EXPECT_THAT(splittedBox.first.maxPoint(), Eq(Point3f(4.f, 5.5f, 3.f)));
  EXPECT_THAT(splittedBox.second.minPoint(), Eq(Point3f(4.f, -2.f, -3.f)));
  EXPECT_THAT(splittedBox.second.maxPoint(), Eq(Point3f(9.f, 5.5f, 3.f)));

  box1 = BoundingBox(Point3f(-1.f, -2.f, -3.f), Point3f(5.f, 8.f, 3.f));
  splittedBox = bvh.splitBoundsOf(box1);
  EXPECT_THAT(splittedBox.first.minPoint(), Eq(Point3f(-1.f, -2.f, -3.f)));
  EXPECT_THAT(splittedBox.first.maxPoint(), Eq(Point3f(5.f, 3.f, 3.f)));
  EXPECT_THAT(splittedBox.second.minPoint(), Eq(Point3f(-1.f, 3.f, -3.f)));
  EXPECT_THAT(splittedBox.second.maxPoint(), Eq(Point3f(5.f, 8.f, 3.f)));

  box1 = BoundingBox(Point3f(-1.f, -2.f, -3.f), Point3f(5.f, 3.f, 7.f));
  splittedBox = bvh.splitBoundsOf(box1);
  EXPECT_THAT(splittedBox.first.minPoint(), Eq(Point3f(-1.f, -2.f, -3.f)));
  EXPECT_THAT(splittedBox.first.maxPoint(), Eq(Point3f(5.f, 3.f, 2.f)));
  EXPECT_THAT(splittedBox.second.minPoint(), Eq(Point3f(-1.f, -2.f, 2.f)));
  EXPECT_THAT(splittedBox.second.maxPoint(), Eq(Point3f(5.f, 3.f, 7.f)));
}

TEST_F(BoundingBoxTest, splitChildrenOfWorld) {
  EntityFactory primitive_builder;
  WorldBuilder world_builder;
  SceneDirector scene_director;

  world_builder.createWorld();

  DataContainer data1;
  data1.setProperty(app::PRIMITIVE_TYPE, app::SPHERE)
      .setProperty(app::COLOR, white)
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN)
      .setProperty(app::TRANSFORMATION_MATRIX, translation(-2.f, 0.f, 0.f));
  scene_director.createSceneElement(primitive_builder, data1);
  auto sphere1 = scene_director.getCurrentElement();

  DataContainer data2;
  data2.setProperty(app::PRIMITIVE_TYPE, app::SPHERE)
      .setProperty(app::COLOR, white)
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN)
      .setProperty(app::TRANSFORMATION_MATRIX, translation(2.f, 0.f, 0.f));
  scene_director.createSceneElement(primitive_builder, data2);
  auto sphere2 = scene_director.getCurrentElement();

  DataContainer data3;
  data3.setProperty(app::PRIMITIVE_TYPE, app::SPHERE)
      .setProperty(app::COLOR, white)
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN);
  scene_director.createSceneElement(primitive_builder, data3);
  auto sphere3 = scene_director.getCurrentElement();

  scene_director.createWorld(world_builder, PointLight());

  auto world = scene_director.getSceneProduct();
  WorldPair wp = bvh.splitElementsOf(world->getChildren(), world->getBounds());

  ASSERT_EQ(world->getChildren().size(), 1);
  ASSERT_TRUE((*world->getChildren().begin()).get() == sphere3.get());

  ASSERT_TRUE(wp.first->getChildren().size() == 1);
  ASSERT_TRUE((*wp.first->getChildren().begin()).get() == sphere1.get());

  ASSERT_TRUE(wp.second->getChildren().size() == 1);
  ASSERT_TRUE((*wp.second->getChildren().begin()).get() == sphere2.get());
}

TEST_F(BoundingBoxTest, divideWorld) {
  WorldBuilder world_builder;
  EntityFactory primitive_builder;
  SceneDirector scene_director;

  DataContainer data1;
  data1.setProperty(app::PRIMITIVE_TYPE, app::SPHERE)
      .setProperty(app::COLOR, white)
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN)
      .setProperty(app::TRANSFORMATION_MATRIX, translation(-2.f, -2.f, 0.f));
  scene_director.createSceneElement(primitive_builder, data1);
  auto sphere1 = scene_director.getCurrentElement();

  DataContainer data2;
  data2.setProperty(app::PRIMITIVE_TYPE, app::SPHERE)
      .setProperty(app::COLOR, white)
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN)
      .setProperty(app::TRANSFORMATION_MATRIX, translation(-2.f, 2.f, 0.f));
  scene_director.createSceneElement(primitive_builder, data2);
  auto sphere2 = scene_director.getCurrentElement();

  DataContainer data3;
  data3.setProperty(app::PRIMITIVE_TYPE, app::SPHERE)
      .setProperty(app::COLOR, white)
      .setProperty(app::TEXTURE_TYPE, app::CONSTANT_TEXTURE)
      .setProperty(app::MATERIAL_TYPE, app::LAMBERTIAN)
      .setProperty(app::TRANSFORMATION_MATRIX, scale(4.f, 4.f, 4.f));
  scene_director.createSceneElement(primitive_builder, data3);
  auto sphere3 = scene_director.getCurrentElement();

  scene_director.createWorld(world_builder, PointLight());

  auto world = scene_director.getSceneProduct();
  bvh.divideWorld(world, 1);
  WorldIterator it(world->getChildren());
  if (it.first()) {
    ASSERT_TRUE(it.currentElement() == sphere3.get());
    ASSERT_FALSE(it.currentElement()->isWorld());
    it.advance();
    ASSERT_TRUE(it.currentElement()->isWorld());
    ASSERT_TRUE(it.currentElement()->getChildren().size() == 2);
    WorldIterator it1(it.currentElement()->getChildren());
    if (it1.first()) {
      ASSERT_TRUE(it1.currentElement()->isWorld());
      ASSERT_TRUE(it1.currentElement()->getChildren().size() == 1);
      ASSERT_TRUE((*it1.currentElement()->getChildren().begin()).get() ==
                  sphere1.get());
      it1.advance();
      ASSERT_TRUE(it1.currentElement()->isWorld());
      ASSERT_TRUE(it1.currentElement()->getChildren().size() == 1);
      ASSERT_TRUE((*it1.currentElement()->getChildren().begin()).get() ==
                  sphere2.get());
    }
  }
}
