#include "composite/world.h"

#include <algorithm>

#include "composite/iterator.h"
#include "renderers/renderer.h"
#include "stochastic/random.h"
#include "world.h"

bool World::intersect(const Ray& r, Intersection& record) {
  if (!m_bBox.intersectsRay(r)) {
    return false;
  }

  WorldIterator it(getChildren());
  if (it.first()) {
    while (it.notDone()) {
      if (it.currentElement()->isWorld()) {
        it.currentElement()->intersect(r, record);
      } else {
        auto rec = Intersection{};
        if (it.currentElement()->intersect(r, rec)) {
          rec.hitFound = true;
          if (rec.min_hit > 0.0f && rec.min_hit < record.minHitParam) {
            rec.primitive = it.currentElement();
            rec.minHitParam = rec.min_hit;
            record = rec;
          }
        }
      }
      it.advance();
    }
  }
  return record.hitFound;
}

void World::add(SceneElementPtr item) {
  item->setParent(this);
  m_bBox.addBox(item->getBoundingBox());
  m_sceneElementContainer.push_back(item);
}

bool World::isWorld() const { return true; }

void World::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElementComposite(this, ray);
}

SceneElementContainer World::getChildren() const {
  return m_sceneElementContainer;
}

SceneElementContainer& World::getChildren() { return m_sceneElementContainer; }

void World::setLight(const PointLight& light) { m_light = light; }

PointLight World::getLight() const { return m_light; }

BoundingBox World::getBoundingBox() const { return m_bBox; }

SceneElementPtr World::create() { return std::make_shared<World>(); }
