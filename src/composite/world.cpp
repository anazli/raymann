#include "composite/world.h"

#include <algorithm>

#include "composite/iterator.h"
#include "geometry/primitive.h"
#include "renderers/renderer.h"
#include "stochastic/random.h"
#include "world.h"

using std::list;
using std::shared_ptr;
using std::sort;
using std::vector;

World::World(const PointLight& light) { m_light = light; }

bool World::intersect(const Ray& r, IntersectionRecord& record) {
  if (!m_bBox.intersectsRay(r)) {
    return false;
  }

  WorldIterator it(getChildren());
  if (it.first()) {
    while (it.notDone()) {
      if (it.currentElement()->isWorld()) {
        it.currentElement()->intersect(r, record);
      } else {
        auto rec = IntersectionRecord{};
        if (it.currentElement()->intersect(r, rec)) {
          rec.hitFound = true;
          if (rec.t_min() > 0.0f && rec.t_min() < record.minHitParam) {
            rec.object = it.currentElement();
            rec.minHitParam = rec.t_min();
            record = rec;
          }
        }
      }
      it.advance();
    }
  }
  return record.hitFound;
}

void World::add(SceneElementPtr element) {
  element->setParent(this);
  if (auto primitive = dynamic_cast<Primitive*>(element.get())) {
    m_bBox.addBox(primitive->boundingBox());
  }
  m_sceneElementContainer.push_back(element);
}

SceneElementContainer::iterator World::remove(SceneElementRawPtr item,
                                              SceneElementPtr elementToRemove) {
  auto it = std::find_if(
      m_sceneElementContainer.begin(), m_sceneElementContainer.end(),
      [&item](const SceneElementPtr& elem) { return item == elem.get(); });
  if (it != m_sceneElementContainer.end()) {
    elementToRemove = *it;
    auto itret = m_sceneElementContainer.erase(it);
    return itret;
  }
  return m_sceneElementContainer.end();
}

bool World::isWorld() const { return true; }

Vec3D World::normal(const Point3D& p) const { return Vec3D(); }

void World::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElementComposite(this, ray);
}

SceneElementContainer World::getChildren() const {
  return m_sceneElementContainer;
}

void World::setLight(const PointLight& light) { m_light = light; }

PointLight World::getLight() const { return m_light; }
