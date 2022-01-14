#include "geometry/sphere.h"
#include "gtest/gtest.h"
#include "tools/tools.h"

using namespace testing;

class TMat : public Test {
 public:
  Sphere *s;
  PointLight light;
};

TEST_F(TMat, createsDefaultLight) {
  ASSERT_TRUE(light.position() == Point3f());
  ASSERT_TRUE(light.intensity() == Vec3f());
}

TEST_F(TMat, createsNewLight1) {
  light.setPosition(Point3f(1.0f, 2.0f, 3.0f));
  light.setIntensity(Vec3f(0.1f, 0.1f, 0.3f));

  ASSERT_EQ(light.position().x(), 1.0f);
  ASSERT_EQ(light.position().y(), 2.0f);
  ASSERT_EQ(light.position().z(), 3.0f);

  ASSERT_EQ(light.intensity().x(), 0.1f);
  ASSERT_EQ(light.intensity().y(), 0.1f);
  ASSERT_EQ(light.intensity().z(), 0.3f);
}

TEST_F(TMat, createsNewLight2) {
  light = PointLight(Point3f(0.1f, -4.0f, -0.4f), Vec3f(1.0f, 4.0f, 0.0f));

  ASSERT_EQ(light.position().x(), 0.1f);
  ASSERT_EQ(light.position().y(), -4.0f);
  ASSERT_EQ(light.position().z(), -0.4f);

  ASSERT_EQ(light.intensity().x(), 1.0f);
  ASSERT_EQ(light.intensity().y(), 4.0f);
  ASSERT_EQ(light.intensity().z(), 0.0f);
}
