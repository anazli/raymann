#include "gtest/gtest.h"
#include "materials/material_properties.h"
#include "tools/tools.h"

using namespace testing;
using namespace std;

class Tprop : public Test {
 public:
  MaterialProperties p;
  Props name = Props::COLOR;  // For some weird reason, with other values like
                              // AMBIENT, DIFFUSE etc.. the tests [addsProperty,
                              // entityHasProperty] fail. Either GoogleTests's
                              // or STL's problem
  any value = 5.6f;
};

TEST_F(Tprop, addsProperty) {
  EXPECT_TRUE(p.addProperty(name, value));
  EXPECT_FALSE(p.addProperty(name, value));
}

TEST_F(Tprop, setsProperty) {
  p.setProperty(name, value);
  EXPECT_TRUE(p.hasProperty(name));
  EXPECT_TRUE(p.getPropertyAsFloat(name) == any_cast<float>(value));
  int new_value(5);
  p.setProperty(name, new_value);
  EXPECT_TRUE(p.getPropertyAsInt(name) == new_value);
}

TEST_F(Tprop, entityHasProperty) {
  EXPECT_FALSE(p.hasProperty(name));
  p.setProperty(name, value);
  EXPECT_TRUE(p.hasProperty(name));
  EXPECT_FALSE(p.hasProperty(static_cast<Props>(1000)));
}

TEST_F(Tprop, getsPropertyAsInt) {
  int new_value = 5;
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAsInt(static_cast<Props>(1000)), 0);
  EXPECT_EQ(p.getPropertyAsInt(name), new_value);
}

TEST_F(Tprop, getsPropertyAsFloat) {
  p.setProperty(name, value);
  EXPECT_EQ(p.getPropertyAsFloat(static_cast<Props>(1000)), 0.f);
  EXPECT_EQ(p.getPropertyAsFloat(name), any_cast<float>(value));
}

TEST_F(Tprop, getsPropertyAsVec3f) {
  Vec3f new_value(1.f, 1.f, 1.f);
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAsVec3f(static_cast<Props>(1000)), Vec3f());
  EXPECT_EQ(p.getPropertyAsVec3f(name), new_value);
}

TEST_F(Tprop, getsPropertyAsMat4f) {
  Mat4f new_value(Vec4<float>(1.36f, 1.28f, 0.85f, -7.f),
                  Vec4<float>(1.5f, 0.f, -6.58f, 1.f),
                  Vec4<float>(4.5f, 0.f, -3.f, 10.f),
                  Vec4<float>(0.f, 1.f, 6.68f, -9.f));
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAsMat4f(static_cast<Props>(1000)), Mat4f());
  EXPECT_EQ(p.getPropertyAsMat4f(name), new_value);
}

TEST_F(Tprop, getsPropertyAsPoint3f) {
  Point3f new_value(1.f, 1.f, 1.f);
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAsPoint3f(static_cast<Props>(1000)), Point3f());
  EXPECT_EQ(p.getPropertyAsPoint3f(name), new_value);
}

TEST_F(Tprop, removesProperty) {
  p.setProperty(name, value);
  EXPECT_TRUE(p.removeProperty(name));
  EXPECT_FALSE(p.removeProperty(name));
}

TEST_F(Tprop, defaultTransparencyAndRefractiveIndexTest) {
  EXPECT_EQ(p.getPropertyAsFloat(Props::TRANSPARENCY), 0.f);
  EXPECT_EQ(p.getPropertyAsFloat(Props::REFRACTIVE_INDEX), 1.f);
}
