#include "geometry/sphere.h"
#include "geometry/traceable.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace testing;

class Tworld : public Test {
 public:
  World w;
};

TEST_F(Tworld, createsWolrdWithShere) {
  w = World();
  Sphere *s = new Sphere(Point3f(0.0f, 0.0f, 0.0f), 1.0f);
  w.add(s);
  ASSERT_TRUE(s->getParent() == &w);
  Ray r = Ray(Point3f(0.0f, 0.0f, -5.0f), Vec3f(0.0f, 0.0f, 1.0f));

  ASSERT_TRUE(w.intersect(r));
  ASSERT_EQ(s->rec.count, 2);
  ASSERT_EQ(s->rec.t1, 4.0f);
  ASSERT_EQ(s->rec.t2, 6.0f);

  w.remove(s);
  ASSERT_FALSE(s->getParent() == &w);
  ASSERT_TRUE(s->getParent() == nullptr);
}
