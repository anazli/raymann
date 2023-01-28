#include "gtest/gtest.h"
#include "tools/tools.h"
#include "composite/properties.h"

using namespace testing;
using namespace std;

class Tprop : public Test {
 public:
  Properties p;
  string name;
  any value;
};

TEST_F(Tprop, entityHasProperty) {
  name = "property";
  value = 5;
  EXPECT_FALSE(p.hasProperty(name));
  p.setProperty(name, value);
  EXPECT_TRUE(p.hasProperty(name));
}
