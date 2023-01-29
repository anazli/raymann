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
  EXPECT_TRUE(p.getPropertyAsFloat(name) == std::any_cast<float>(value));
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
