#include "container/canvas.h"

#include <gtest/gtest.h>

class CanvasTest : public testing::Test {
 public:
  Canvas c;
};

TEST_F(CanvasTest, createsCanvas) {
  c = Canvas(10, 20);
  ASSERT_EQ(c.width(), 10);
  ASSERT_EQ(c.height(), 20);
  ASSERT_TRUE(c.pixel(5, 5) == Vec3f(0., 0., 0.));

  c = Canvas(50, 90);
  ASSERT_EQ(c.width(), 50);
  ASSERT_EQ(c.height(), 90);
  ASSERT_TRUE(c.pixel(40, 50) == Vec3f(0., 0., 0.));
}

TEST_F(CanvasTest, setsPixelColor) {
  c = Canvas(800, 600);
  ASSERT_EQ(c.width(), 800);
  ASSERT_EQ(c.height(), 600);
  c.writePixel(500, 500, Vec3f(1., 0., 0));
  ASSERT_TRUE(c.pixel(500, 500) == Vec3f(1., 0., 0.));
  ASSERT_TRUE(c.pixel(100, 500) == Vec3f(0., 0., 0.));
  ASSERT_TRUE(c.pixel(799, 599) == Vec3f(0., 0., 0.));
  ASSERT_TRUE(c.pixel(0, 0) == Vec3f(0., 0., 0.));
}

TEST_F(CanvasTest, savesCanvas) {
  c = Canvas(5, 3);
  Vec3f color1(1.5, 0., 0.);
  Vec3f color2(0., 0.5, 0.);
  Vec3f color3(-0.5, 0., 1.);

  c.writePixel(0, 0, color1);
  c.writePixel(2, 1, color2);
  c.writePixel(4, 2, color3);
  ASSERT_DEATH(c.save(), "");
  c.setFileName("test1.ppm");
  c.save();
}
