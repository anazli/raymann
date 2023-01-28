#include "gtest/gtest.h"
#include "tools/tools.h"
#include "composite/properties.h"

using namespace testing;
using namespace std;

class Tprop : public Test {
 public:
  Properties p;
  string name = "property";
  any value = 5.6;
};

TEST_F(Tprop, addsProperty){
  EXPECT_TRUE(p.addProperty(name, value));
  EXPECT_FALSE(p.addProperty("", 5));
}

TEST_F(Tprop, setsProperty){
}

TEST_F(Tprop, entityHasProperty) {
  EXPECT_FALSE(p.hasProperty(name));
  p.setProperty(name, value);
  EXPECT_TRUE(p.hasProperty(name));
  EXPECT_FALSE(p.hasProperty(""));
}
