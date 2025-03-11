#include "composite/scene_element.h"

#include "iterator.h"
#include "renderers/renderer.h"
#include "scene_element.h"
#include "world.h"

bool SceneElement::intersect(const Ray& r, Intersection& record) {
  if (getBoundingBox().intersectsRay(r))
    return m_geometric_primitive->intersect(r, record);
  return false;
}

void SceneElement::add(SceneElementPtr item) {}

bool SceneElement::isWorld() const { return false; }

void SceneElement::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElementLeaf(this, ray);
}

SceneElementContainer SceneElement::getChildren() const {
  return SceneElementContainer();
}

std::vector<std::shared_ptr<SceneElement> >& SceneElement::getChildren() {}

void SceneElement::setMaterial(MaterialPtr mat) { m_material = mat; }

const MaterialRawPtr SceneElement::getMaterial() const {
  return m_material.get();
}

void SceneElement::setPrimitive(PrimitivePtr pr) { m_geometric_primitive = pr; }

void SceneElement::setParent(SceneElementRawPtr parent) { m_parent = parent; }

SceneElementRawPtr SceneElement::getParent() const { return m_parent; }

void SceneElement::setLight(const PointLight& light) {}

PointLight SceneElement::getLight() const { return PointLight(); }

BoundingBox SceneElement::getBoundingBox() const {
  return m_geometric_primitive->worldBounds();
}
