#include "geometry/sphere.h"
#include "gtest/gtest.h"
#include "world/world.h"

using namespace testing;

TEST(SphereBuilder, CreateSimpleSphere) {
  std::unique_ptr<World> world;
  std::shared_ptr<StandardSphere> builder;

  world->createSphere(builder);
  std::shared_ptr<Traceable> sphere = builder.getSphere();
}
