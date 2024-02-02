#include "acceleration/scene_element_proxy.h"
#include "composite/builder.h"
#include "geometry/sphere.h"
#include "gtest/gtest.h"

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
