#include "camera/camera.h"
#include "gtest/gtest.h"
#include "tools/tools.h"

using namespace testing;

class Tcam : public Test {
 public:
  Camera c;
};

TEST_F(Tcam, createsCamera) {
  c = Camera();
  c.setHSize(160);
  c.setVSize(120);
  c.setFieldOfView(PI / 2.0f);

  ASSERT_EQ(c.hSize(), 160);
  ASSERT_EQ(c.vSize(), 120);
  ASSERT_EQ(c.fieldOfView(), PI / 2.0f);
  ASSERT_TRUE(c.transform() == Mat4f());
}

TEST_F(Tcam, pixelSizeForHorCanvas) {
  c = Camera(200, 125, PI / 2.0f);
  c.computePixelSize();
  ASSERT_EQ(c.pixelSize(), 0.01f);
}

TEST_F(Tcam, pixelSizeForVerCanvas) {
  c = Camera(125, 200, PI / 2.0f);
  c.computePixelSize();
  ASSERT_EQ(c.pixelSize(), 0.01f);
}

TEST_F(Tcam, rayThroughTheCenterOfCanvas) {
  c = Camera(201, 101, PI / 2.0f);
  Ray r = Ray(c, 100, 50);
  float eps = 1.E-9;
  EXPECT_NEAR(r.origin().x(), 0.0f, eps);
  EXPECT_NEAR(r.origin().y(), 0.0f, eps);
  EXPECT_NEAR(r.origin().z(), 0.0f, eps);
  EXPECT_NEAR(r.direction().x(), 0.0f, eps);
  EXPECT_NEAR(r.direction().y(), 0.0f, eps);
  EXPECT_NEAR(r.direction().z(), -1.0f, eps);
}
