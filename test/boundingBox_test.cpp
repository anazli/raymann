#include "acceleration/scene_element_proxy.h"
#include "gtest/gtest.h"

using namespace testing;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::vector;

class Tworld : public Test {
 public:
  SceneElementPtr box1;
  SceneElementPtr box2;
  MaterialProperties prop;
  IntersectionRecord rec;
};

TEST_F(Tworld, createsWorldOfShere) {
  box1 = std::make_shared<SceneElementProxy>(Point3f(-5.f, -2.f, 0.f),
                                             Point3f(7.f, 4.f, 4.f));
  box2 = std::make_shared<SceneElementProxy>(Point3f(8.f, -7.f, -2.f),
                                             Point3f(14.f, 2.f, 8.f));
}
