#include "composite/scene_element.h"

#include "renderers/renderer.h"

size_t SceneElement::m_next_id = 0;

bool SceneElement::intersect(const Ray& r, IntersectionRecord& record) {
  return false;
}

void SceneElement::add(SceneElementPtr item) {}

void SceneElement::remove(SceneElementRawPtr item, bool del) {}

bool SceneElement::isWorld() const { return false; }

Vec3f SceneElement::normal(const Point3f& p) const { return Vec3f(); }

void SceneElement::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElementLeaf(this, ray);
}

std::list<SceneElementPtr> SceneElement::getWorldList() {
  return std::list<SceneElementPtr>();
}

void SceneElement::setMaterial(BaseMaterialPtr mat) { m_material = mat; }

BaseMaterialPtr SceneElement::getMaterial() const { return m_material; }

void SceneElement::setParent(SceneElementRawPtr parent) { m_parent = parent; }

SceneElementRawPtr SceneElement::getParent() const { return m_parent; }

void SceneElement::setLight(const PointLight& light) {}

PointLight SceneElement::getLight() const { return PointLight(); }

size_t SceneElement::getId() const { return m_id; }

Point3f SceneElement::pointFromWorldToObjectSpace(const Point3f& point) const {
  return Point3f();
}

Vec3f SceneElement::vectorFromObjectToWorldSpace(const Vec3f vec) const {
  return Vec3f();
}

SceneElement::SceneElement() {
  m_id = m_next_id;
  m_next_id++;
}
