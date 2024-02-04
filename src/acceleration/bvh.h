#pragma once

#include "acceleration/bounding_box.h"

using BoundingBoxPair = std::pair<BoundingBox, BoundingBox>;

class BVHierarchy {
 public:
  BoundingBoxPair splitBoundsOf(const BoundingBox &box);

 private:
};
