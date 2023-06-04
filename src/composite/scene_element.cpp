#include "composite/scene_element.h"

#include "renderers/renderer.h"

bool SceneElement::isWorld() const { return false; }

Vec3f SceneElement::normal(const Point3f& p) const { return Vec3f(); }

void SceneElement::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElement(*this, ray);
}

void SceneElement::setRecord(const IntersectionRecord& rec) { m_rec = rec; }
IntersectionRecord& SceneElement::getRecord() { return m_rec; }

void SceneElement::setMaterial(BaseMaterialPtr mat) { m_material = mat; }

BaseMaterialPtr SceneElement::getMaterial() const { return m_material; }

void SceneElement::setParent(std::shared_ptr<SceneElement> t) { m_parent = t; }

std::shared_ptr<SceneElement> SceneElement::getParent() const {
  return m_parent;
}
