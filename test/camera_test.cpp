#include "camera/camera.h"

#include "utils.h"

using testing::Eq;
using testing::FloatEq;

class CameraTest : public testing::Test {
 public:
  Camera c;
  float eps = 1.E-5;
};

TEST_F(CameraTest, createsCamera) {
  ASSERT_THAT(c.getTransform(), Eq(Mat4D()));
}

TEST_F(CameraTest, pixelSizeForHorCanvas) {
  c = Camera(200, 125, PI / 2.0f);
  c.computePixelSize();
  ASSERT_THAT(c.pixelSize(), FloatEq(0.01f));
}

TEST_F(CameraTest, pixelSizeForVerCanvas) {
  c = Camera(125, 200, PI / 2.0f);
  c.computePixelSize();
  ASSERT_THAT(c.pixelSize(), FloatEq(0.01f));
}

TEST_F(CameraTest, rayThroughTheCenterOfCanvas) {
  c = Camera(201, 101, PI / 2.0f);
  c.computePixelSize();
  Ray r = c.getRay(100, 50);
  EXPECT_THAT(r.origin(), Point3Near(Point3f(), eps));
  EXPECT_THAT(r.direction(), Vec3Near(Vec3f(0.f, 0.f, -1.f), eps));
}

TEST_F(CameraTest, rayThroughCornerOfCanvas) {
  c = Camera(201, 101, PI / 2.0f);
  c.computePixelSize();
  Ray r = c.getRay(0, 0);
  EXPECT_THAT(r.origin(), Point3Near(Point3f(), eps));
  EXPECT_THAT(r.direction(),
              Vec3Near(Vec3f(0.66519f, 0.33259f, -0.66851f), eps));
}

TEST_F(CameraTest, rayWhenCameraIsTransformed) {
  c = Camera(201, 101, PI / 2.0f);
  c.setTransform(rotationOverY(PI / 4.0f) * translation(0.0f, -2.0f, 5.0f));
  c.computePixelSize();
  Ray r = c.getRay(100, 50);
  EXPECT_THAT(r.origin(), Point3Near(Point3f(0.f, 2.f, -5.f), eps));
  EXPECT_THAT(r.direction(), Vec3Near(Vec3f(0.7071f, 0.f, -0.7071f), eps));
}
