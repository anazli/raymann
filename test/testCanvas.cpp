#include "canvas.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace testing;

class Tcanvas : public Test {
 public:
  Canvas c;
};

TEST_F(Tcanvas, createsCanvas) {
  c = Canvas(10, 20);
  ASSERT_EQ(c.width(), 10);
  ASSERT_EQ(c.height(), 20);
  ASSERT_TRUE(c.pixel(5, 5) == Vec3f(0., 0., 0.));

  c = Canvas(50, 90);
  ASSERT_EQ(c.width(), 50);
  ASSERT_EQ(c.height(), 90);
  ASSERT_TRUE(c.pixel(40, 50) == Vec3f(0., 0., 0.));
}
