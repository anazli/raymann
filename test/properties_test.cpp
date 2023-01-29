#include "composite/properties.h"

#include "gtest/gtest.h"
#include "tools/tools.h"

using namespace testing;
using namespace std;

class Tprop : public Test {
 public:
  Properties p;
  string name = "property";
  any value = 5.6f;
};

TEST_F(Tprop, addsProperty) {
  EXPECT_TRUE(p.addProperty(name, value));
  EXPECT_FALSE(p.addProperty("", value));
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
  EXPECT_FALSE(p.hasProperty(""));
}

TEST_F(Tprop, getPropertyAsInt) {
  int new_value = 5;
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAsInt(""), 0);
  EXPECT_EQ(p.getPropertyAsInt(name), new_value);
}

TEST_F(Tprop, getPropertyAsFloat) {
  p.setProperty(name, value);
  EXPECT_EQ(p.getPropertyAsFloat(""), 0.f);
  EXPECT_EQ(p.getPropertyAsFloat(name), any_cast<float>(value));
}

TEST_F(Tprop, getPropertyAsVec3f) {
  Vec3f new_value(1.f, 1.f, 1.f);
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAsVec3f(""), Vec3f());
  EXPECT_EQ(p.getPropertyAsVec3f(name), new_value);
}

TEST_F(Tprop, getPropertyAsMat4f) {
  Mat4f new_value(Vec4<float>(1.36f, 1.28f, 0.85f, -7.f),
                  Vec4<float>(1.5f, 0.f, -6.58f, 1.f),
                  Vec4<float>(4.5f, 0.f, -3.f, 10.f),
                  Vec4<float>(0.f, 1.f, 6.68f, -9.f));
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAsMat4f(""), Mat4f());
  EXPECT_EQ(p.getPropertyAsMat4f(name), new_value);
}

TEST_F(Tprop, getPropertyAsPoint3f) {
  Point3f new_value(1.f, 1.f, 1.f);
  p.setProperty(name, new_value);
  EXPECT_EQ(p.getPropertyAsPoint3f(""), Point3f());
  EXPECT_EQ(p.getPropertyAsPoint3f(name), new_value);
}
