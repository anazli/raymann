#pragma once

#include "acceleration/bounding_box.h"
#include "composite/scene_element.h"
#include "composite/world.h"

using BoundingBoxPair = std::pair<BoundingBox, BoundingBox>;
using WorldPair = std::pair<SceneElementPtr, SceneElementPtr>;

class BVHierarchy {
 public:
  BoundingBoxPair splitBoundsOf(const BoundingBox &box);
  WorldPair splitElementsOf(SceneElementContainer &worldList,
                            const BoundingBox &worldBox);

 private:
};
