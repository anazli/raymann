#include "geometry/sphere.h"
#include "gtest/gtest.h"
#include "world/world.h"

using namespace testing;

TEST(SphereBuilder, CreateSimpleSphere) {
  std::unique_ptr<World> world;
  std::shared_ptr<StandardSphere> builder;

  Traceable *test = world->createSphere(builder);
  std::shared_ptr<Traceable> sphere =
      builder->getSphere();  // not really neccessary, in case we want to use
                             // sphere in another world

  ASSERT_TRUE(test == sphere.get());
}
