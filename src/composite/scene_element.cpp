#include "composite/scene_element.h"

#include "renderers/renderer.h"
#include "world.h"

void SceneElement::add(SceneElementPtr element) {}

SceneElementContainer::iterator SceneElement::remove(
    SceneElementRawPtr item, SceneElementPtr elementToRemove) {}

bool SceneElement::isWorld() const { return false; }

void SceneElement::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElementLeaf(this, ray);
}

SceneElementContainer SceneElement::getChildren() const {
  return SceneElementContainer();
}

void SceneElement::setParent(SceneElementRawPtr parent) { m_parent = parent; }

SceneElementRawPtr SceneElement::getParent() const { return m_parent; }
