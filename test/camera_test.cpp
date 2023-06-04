#include "camera/camera.h"

#include "gtest/gtest.h"
#include "tools/tools.h"

using namespace testing;

class Tcam : public Test {
 public:
  RayTracingChalengeCamera c;
};

TEST_F(Tcam, createsCamera) {
  c = RayTracingChalengeCamera();
  c.setHSize(160);
  c.setVSize(120);
  c.setFieldOfView(PI / 2.0f);

  ASSERT_EQ(c.hSize(), 160);
  ASSERT_EQ(c.vSize(), 120);
  ASSERT_EQ(c.fieldOfView(), PI / 2.0f);
  ASSERT_TRUE(c.transform() == Mat4f());
}

TEST_F(Tcam, pixelSizeForHorCanvas) {
  c = RayTracingChalengeCamera(200, 125, PI / 2.0f);
  c.computePixelSize();
  ASSERT_EQ(c.pixelSize(), 0.01f);
}

TEST_F(Tcam, pixelSizeForVerCanvas) {
  c = RayTracingChalengeCamera(125, 200, PI / 2.0f);
  c.computePixelSize();
  ASSERT_EQ(c.pixelSize(), 0.01f);
}

TEST_F(Tcam, rayThroughTheCenterOfCanvas) {
  c = RayTracingChalengeCamera(201, 101, PI / 2.0f);
  c.computePixelSize();
  Ray r = c.getRay(100, 50);
  float eps = 1.E-6;
  EXPECT_NEAR(r.origin().x(), 0.0f, eps);
  EXPECT_NEAR(r.origin().y(), 0.0f, eps);
  EXPECT_NEAR(r.origin().z(), 0.0f, eps);
  EXPECT_NEAR(r.direction().x(), 0.0f, eps);
  EXPECT_NEAR(r.direction().y(), 0.0f, eps);
  EXPECT_NEAR(r.direction().z(), -1.0f, eps);
}

TEST_F(Tcam, rayThroughCornerOfCanvas) {
  c = RayTracingChalengeCamera(201, 101, PI / 2.0f);
  c.computePixelSize();
  Ray r = c.getRay(0, 0);
  float eps = 1.E-5;
  EXPECT_NEAR(r.origin().x(), 0.0f, eps);
  EXPECT_NEAR(r.origin().y(), 0.0f, eps);
  EXPECT_NEAR(r.origin().z(), 0.0f, eps);
  EXPECT_NEAR(r.direction().x(), 0.66519f, eps);
  EXPECT_NEAR(r.direction().y(), 0.33259f, eps);
  EXPECT_NEAR(r.direction().z(), -0.66851f, eps);
}

TEST_F(Tcam, rayWhenCameraIsTransformed) {
  c = RayTracingChalengeCamera(201, 101, PI / 2.0f);
  c.setTransform(rotY(PI / 4.0f) * transl(0.0f, -2.0f, 5.0f));
  c.computePixelSize();
  Ray r = c.getRay(100, 50);
  float eps = 1.E-5;
  EXPECT_NEAR(r.origin().x(), 0.0f, eps);
  EXPECT_NEAR(r.origin().y(), 2.0f, eps);
  EXPECT_NEAR(r.origin().z(), -5.0f, eps);
  EXPECT_NEAR(r.direction().x(), 0.7071f, eps);
  EXPECT_NEAR(r.direction().y(), 0.0f, eps);
  EXPECT_NEAR(r.direction().z(), -0.7071f, eps);
}
