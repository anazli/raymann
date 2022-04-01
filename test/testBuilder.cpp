#include "geometry/sphere.h"
#include "gtest/gtest.h"
#include "world/scene.h"
#include "world/world.h"

using namespace testing;

TEST(SphereBuilder, CreateSimpleSphere) {
  Scene scene;
  std::shared_ptr<StandardSphere> builder;

  std::shared_ptr<Traceable> test = scene.createSphere(builder);
  std::shared_ptr<Traceable> sphere = builder->getSphere();

  ASSERT_TRUE(test == sphere);
}
