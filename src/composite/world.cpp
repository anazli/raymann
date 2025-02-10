#include "composite/world.h"

#include <algorithm>

#include "composite/iterator.h"
#include "renderers/renderer.h"
#include "stochastic/random.h"
#include "world.h"

using std::list;
using std::shared_ptr;
using std::sort;
using std::vector;

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

void World::add(SceneElementPtr item) {
  item->setParent(this);
  m_bBox.addBox(item->getBoundingBox());
  m_sceneElementContainer.push_back(item);
}

SceneElementContainer::iterator World::remove(SceneElementRawPtr item,
                                              SceneElementPtr removedElem) {
  auto it = std::find_if(
      m_sceneElementContainer.begin(), m_sceneElementContainer.end(),
      [&item](const SceneElementPtr& elem) { return item == elem.get(); });
  if (it != m_sceneElementContainer.end()) {
    removedElem = *it;
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

SceneElementContainer& World::getChildren() { return m_sceneElementContainer; }

void World::setLight(const PointLight& light) { m_light = light; }

PointLight World::getLight() const { return m_light; }

float World::pdf(const Point3D& origin, const Vec3D& direction) {
  auto weight = 1.f / m_sceneElementContainer.size();
  auto sum = 0.f;

  for (const auto& object : m_sceneElementContainer)
    sum += weight * object->pdf(origin, direction);

  return sum;
}

Vec3D World::random(const Point3D& origin) {
  auto int_size = static_cast<int>(m_sceneElementContainer.size());
  return m_sceneElementContainer[Random::randomInteger(0, int_size - 1)]
      ->random(origin);
}

SceneElementPtr World::create() { return std::make_shared<World>(); }
