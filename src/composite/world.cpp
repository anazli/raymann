#include "composite/world.h"

#include <algorithm>

#include "renderers/renderer.h"

using std::list;
using std::shared_ptr;
using std::sort;
using std::vector;

bool World::intersect(const Ray& r, IntersectionRecord& record) {
  WorldIterator it(getChildren());
  float minHitParam = MAXFLOAT;
  bool hitFound = false;
  if (it.first()) {
    while (it.notDone()) {
      auto rec = IntersectionRecord{};
      if (it.currentElement()->intersect(r, rec)) {
        hitFound = true;
        if (rec.t_min() > 0.0f && rec.t_min() < minHitParam) {
          rec.object = it.currentElement();
          minHitParam = rec.t_min();
          record = rec;
        }
      }
      it.advance();
    }
  }
  return hitFound;
}

void World::add(SceneElementPtr item) {
  item->setParent(this);
  m_sceneElementContainer.push_back(item);
}

void World::remove(SceneElementRawPtr item, bool del) {
  m_sceneElementContainer.erase(std::remove_if(
      m_sceneElementContainer.begin(), m_sceneElementContainer.end(),
      [item](const SceneElementPtr& elem) { return item == elem.get(); }));
}

bool World::isWorld() const { return true; }

Vec3f World::normal(const Point3f& p) const { return Vec3f(); }

void World::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElementComposite(this, ray);
}

SceneElementContainer World::getChildren() { return m_sceneElementContainer; }

void World::setLight(const PointLight& light) { m_light = light; }

PointLight World::getLight() const { return m_light; }
