#include "geometry/sphere.h"
#include "geometry/traceable.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace testing;

class Tworld : Test {
 public:
  World w;
  Sphere s;
};
