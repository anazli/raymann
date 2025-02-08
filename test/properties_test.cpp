#include "gtest/gtest.h"
#include "materials/material_properties.h"
#include "tools/tools.h"

using namespace testing;
using namespace std;

class MaterialPropertiesTest : public Test {
 public:
  MaterialProperties p;
  MaterialProperties::Properties name =
      MaterialProperties::COLOR;  // For some weird reason, with other values
                                  // like AMBIENT, DIFFUSE etc.. the tests
                                  // [addsProperty, entityHasProperty] fail.
                                  // Either GoogleTests's or STL's problem
  std::any value = 5.6f;
};

TEST_F(MaterialPropertiesTest, addsProperty) {
  EXPECT_TRUE(p.addProperty(name, value));
  EXPECT_FALSE(p.addProperty(name, value));
}

TEST_F(MaterialPropertiesTest, setsProperty) {
  p.setProperty(name, value);
  EXPECT_TRUE(p.hasProperty(name));
  EXPECT_TRUE(p.getPropertyAs<float>(name).value() == any_cast<float>(value));
  int new_value(5);
  p.setProperty(name, new_value);
  EXPECT_TRUE(p.getPropertyAs<int>(name).value() == new_value);
}

TEST_F(MaterialPropertiesTest, entityHasProperty) {
  EXPECT_FALSE(p.hasProperty(name));
  p.setProperty(name, value);
  EXPECT_TRUE(p.hasProperty(name));
  EXPECT_FALSE(
      p.hasProperty(static_cast<MaterialProperties::Properties>(1000)));
}

TEST_F(MaterialPropertiesTest, getsPropertyAsInt) {
  int new_value = 5;
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAs<int>(name).value(), new_value);
}

TEST_F(MaterialPropertiesTest, getsPropertyAsFloat) {
  p.setProperty(name, value);
  EXPECT_EQ(p.getPropertyAs<float>(name).value(), any_cast<float>(value));
}

TEST_F(MaterialPropertiesTest, getsPropertyAsVec3D) {
  Vec3D new_value(1.f, 1.f, 1.f);
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAs<Vec3D>(name).value(), new_value);
}

TEST_F(MaterialPropertiesTest, getsPropertyAsMat4D) {
  Mat4D new_value(Vec4<float>(1.36f, 1.28f, 0.85f, -7.f),
                  Vec4<float>(1.5f, 0.f, -6.58f, 1.f),
                  Vec4<float>(4.5f, 0.f, -3.f, 10.f),
                  Vec4<float>(0.f, 1.f, 6.68f, -9.f));
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAs<Mat4D>(name).value(), new_value);
}

TEST_F(MaterialPropertiesTest, getsPropertyAsPoint3D) {
  Point3D new_value(1.f, 1.f, 1.f);
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAs<Point3D>(name).value(), new_value);
}

TEST_F(MaterialPropertiesTest, removesProperty) {
  p.setProperty(name, value);
  EXPECT_TRUE(p.removeProperty(name));
  EXPECT_FALSE(p.removeProperty(name));
}

TEST_F(MaterialPropertiesTest, defaultTransparencyAndRefractiveIndexTest) {
  EXPECT_EQ(p.getPropertyAs<float>(MaterialProperties::TRANSPARENCY).value(),
            0.f);
  EXPECT_EQ(
      p.getPropertyAs<float>(MaterialProperties::REFRACTIVE_INDEX).value(),
      1.f);
}
