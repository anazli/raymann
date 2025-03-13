#pragma once

#include "acceleration/bounding_box.h"
#include "composite/scene_element.h"

using BoundingBoxPair = std::pair<BoundingBox, BoundingBox>;
using WorldPair = std::pair<SceneElementPtr, SceneElementPtr>;

class BVHierarchy {
 public:
  BoundingBoxPair splitBoundsOf(const BoundingBox &box) const;
  WorldPair splitElementsOf(SceneElementContainer &worldList,
                            const BoundingBox &worldBox) const;
  void divideWorld(const SceneElementPtr &world, size_t threshold = 1) const;
};
