#include "composite/scene_element.h"

#include "iterator.h"
#include "renderers/renderer.h"
#include "scene_element.h"

SceneElement::SceneElement() {}

bool SceneElement::intersect(const Ray& r, Intersection& record) {
  if (!m_bBox.intersectsRay(r)) {
    return false;
  }

  Intersection closest_hit = record;
  bool hit_found = false;
  if (isWorld()) {
    for (const auto& child : getChildren()) {
      if (child->intersect(r, closest_hit)) {
        hit_found = true;
        if (closest_hit.min_hit < record.min_hit) {
          record = closest_hit;
        }
      }
    }
  } else {
    if (m_geometric_primitive->intersect(r, closest_hit) &&
        (closest_hit.min_hit >= r.getMinRange() &&
         closest_hit.min_hit < r.getMaxRange())) {
      hit_found = true;
      if (closest_hit.min_hit < record.min_hit) {
        closest_hit.closest_scene_element = this;
        record = closest_hit;
      }
    }
  }
  return hit_found;
}

void SceneElement::add(SceneElementPtr item) {
  item->setParent(this);
  m_bBox.addBox(item->getBoundingBox());
  m_children.emplace_back(item);
}

bool SceneElement::isWorld() const { return !m_children.empty(); }

void SceneElement::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElement(this, ray);
}

std::vector<std::shared_ptr<SceneElement>> SceneElement::getChildren() const {
  return m_children;
}

std::vector<std::shared_ptr<SceneElement>>& SceneElement::getChildren() {
  return m_children;
}

void SceneElement::setMaterial(MaterialPtr mat) { m_material = mat; }

const MaterialRawPtr SceneElement::getMaterial() const {
  return m_material.get();
}

void SceneElement::setPrimitive(PrimitivePtr pr) {
  m_geometric_primitive = pr;
  m_bBox = m_geometric_primitive->worldBounds();
}

PrimitivePtr SceneElement::getPrimitive() { return m_geometric_primitive; }

std::shared_ptr<SceneElement> SceneElement::create() {
  return std::make_shared<SceneElement>();
}

void SceneElement::setParent(SceneElementRawPtr parent) { m_parent = parent; }

SceneElementRawPtr SceneElement::getParent() const { return m_parent; }

void SceneElement::setLight(const PointLight& light) { m_light = light; }

PointLight SceneElement::getLight() const { return m_light; }

BoundingBox SceneElement::getBoundingBox() const { return m_bBox; }
