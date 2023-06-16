#include "composite/scene_element.h"

#include "renderers/renderer.h"

size_t SceneElement::m_next_id = 0;

bool SceneElement::isWorld() const { return false; }

Vec3f SceneElement::normal(const Point3f& p) const { return Vec3f(); }

void SceneElement::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElement(*this, ray);
}

void SceneElement::setMaterial(BaseMaterialPtr mat) { m_material = mat; }

BaseMaterialPtr SceneElement::getMaterial() const { return m_material; }

void SceneElement::setParent(std::shared_ptr<SceneElement> t) { m_parent = t; }

std::shared_ptr<SceneElement> SceneElement::getParent() const {
  return m_parent;
}

size_t SceneElement::getId() const { return m_id; }

SceneElement::SceneElement() {
  m_id = m_next_id;
  m_next_id++;
}