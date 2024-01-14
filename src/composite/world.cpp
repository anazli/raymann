#include "composite/world.h"

#include <algorithm>

#include "renderers/renderer.h"

using std::list;
using std::shared_ptr;
using std::sort;
using std::vector;

bool World::intersect(const Ray& r, IntersectionRecord& record) {
  return false;
}

void World::add(SceneElementPtr item) {
  item->setParent(shared_from_this());
  m_sceneElementContainer.push_back(item);
}

void World::remove(SceneElementPtr item, bool del) {
  m_sceneElementContainer.remove(item);
  item->setParent(nullptr);
  if (del) item.reset();
}

bool World::isWorld() const { return true; }

Vec3f World::normal(const Point3f& p) const { return Vec3f(); }

void World::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElementComposite(shared_from_this(), ray);
}

SceneElementContainer World::getWorldList() { return m_sceneElementContainer; }

void World::setLight(const PointLight& light) { m_light = light; }

PointLight World::getLight() const { return m_light; }
