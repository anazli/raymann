#include "camera/camera.h"

#include "gtesting.h"

class CameraTest : public testing::RTest {
 public:
  Camera c;
};

TEST_F(CameraTest, createsCamera) {
  c = Camera(0.f, 0.f, 0.f);
  c.setHSize(160);
  c.setVSize(120);
  c.setFieldOfView(PI / 2.0f);

  ASSERT_EQ(c.hSize(), 160);
  ASSERT_EQ(c.vSize(), 120);
  ASSERT_EQ(c.fieldOfView(), PI / 2.0f);
  ASSERT_TRUE(c.transform() == Mat4D());
}

TEST_F(CameraTest, pixelSizeForHorCanvas) {
  c = Camera(200, 125, PI / 2.0f);
  c.computePixelSize();
  ASSERT_EQ(c.pixelSize(), 0.01f);
}

TEST_F(CameraTest, pixelSizeForVerCanvas) {
  c = Camera(125, 200, PI / 2.0f);
  c.computePixelSize();
  ASSERT_EQ(c.pixelSize(), 0.01f);
}

TEST_F(CameraTest, rayThroughTheCenterOfCanvas) {
  c = Camera(201, 101, PI / 2.0f);
  c.computePixelSize();
  Ray r = c.getRay(100, 50);
  float eps = 1.E-6;
  comparePointsApprox(r.origin(), Point3D(), eps);
  compareVectorsApprox(r.direction(), Vec3D(0.f, 0.f, -1.f), eps);
}

TEST_F(CameraTest, rayThroughCornerOfCanvas) {
  c = Camera(201, 101, PI / 2.0f);
  c.computePixelSize();
  Ray r = c.getRay(0, 0);
  float eps = 1.E-5;
  comparePointsApprox(r.origin(), Point3D(), eps);
  compareVectorsApprox(r.direction(), Vec3D(0.66519f, 0.33259f, -0.66851f),
                       eps);
}

TEST_F(CameraTest, rayWhenCameraIsTransformed) {
  c = Camera(201, 101, PI / 2.0f);
  c.setTransform(rotationOverY(PI / 4.0f) * translation(0.0f, -2.0f, 5.0f));
  c.computePixelSize();
  Ray r = c.getRay(100, 50);
  float eps = 1.E-5;
  comparePointsApprox(r.origin(), Point3D(0.f, 2.f, -5.f), eps);
  compareVectorsApprox(r.direction(), Vec3D(0.7071f, 0.f, -0.7071f), eps);
}
