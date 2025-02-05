#include "acceleration/bvh.h"

#include "composite/builder.h"
#include "composite/iterator.h"

BoundingBoxPair BVHierarchy::splitBoundsOf(const BoundingBox &box) const {
  auto dx = box.maxPoint().x() - box.minPoint().x();
  auto dy = box.maxPoint().y() - box.minPoint().y();
  auto dz = box.maxPoint().z() - box.minPoint().z();

  auto m = std::max(std::max(dx, dy), dz);

  auto x0 = box.minPoint().x();
  auto y0 = box.minPoint().y();
  auto z0 = box.minPoint().z();

  auto x1 = box.maxPoint().x();
  auto y1 = box.maxPoint().y();
  auto z1 = box.maxPoint().z();

  if (m == dx) {
    x1 = x0 + dx / 2.f;
    x0 = x1;
  } else if (m == dy) {
    y1 = y0 + dy / 2.f;
    y0 = y1;
  } else if (m == dz) {
    z1 = z0 + dz / 2.f;
    z0 = z1;
  }

  auto midMin = Point3D(x0, y0, z0);
  auto midMax = Point3D(x1, y1, z1);

  auto left = BoundingBox(box.minPoint(), midMax);
  auto right = BoundingBox(midMin, box.maxPoint());

  return std::make_pair(left, right);
}

WorldPair BVHierarchy::splitElementsOf(SceneElementContainer &worldList,
                                       const BoundingBox &worldBox) const {
  BoundingBoxPair boxPair = splitBoundsOf(worldBox);

  SceneElementPtr leftWorld = std::make_shared<World>();
  SceneElementPtr rightWorld = std::make_shared<World>();

  WorldPair worldPair(leftWorld, rightWorld);
  SceneElementContainer::iterator it = worldList.begin();
  while (it != worldList.end()) {
    if (*it != nullptr) {
      if (boxPair.first.containsBoundingBox((*it)->boundingBox())) {
        SceneElementPtr removedElem = *it;
        it = worldList.erase(it);
        if (removedElem) {
          worldPair.first->add(removedElem);
          worldPair.first->setLight(removedElem->getParent()->getLight());
        }
      } else if (boxPair.second.containsBoundingBox((*it)->boundingBox())) {
        SceneElementPtr removedElem = *it;
        it = worldList.erase(it);
        if (removedElem) {
          worldPair.second->add(removedElem);
          worldPair.second->setLight(removedElem->getParent()->getLight());
        }
      } else {
        it++;
      }
    }
  }
  return worldPair;
}

void BVHierarchy::divideWorld(const SceneElementPtr &world,
                              size_t threshold) const {
  if (world->isWorld()) {
    if (world->getChildren().size() >= threshold) {
      WorldPair worldpair =
          splitElementsOf(world->getChildren(), world->boundingBox());
      if (!worldpair.first->getChildren().empty()) {
        world->add(worldpair.first);
      }
      if (!worldpair.second->getChildren().empty()) {
        world->add(worldpair.second);
      }
    }
    if (world->getChildren().empty()) return;
    for (const auto &elem : world->getChildren()) {
      divideWorld(elem, threshold);
    }
  }
}
