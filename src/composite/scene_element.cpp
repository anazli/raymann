#include "composite/scene_element.h"

#include "iterator.h"
#include "renderers/renderer.h"
#include "scene_element.h"

SceneElementNode::SceneElementNode() {}

bool SceneElementNode::intersect(const Ray& r, Intersection& record) {
  if (!m_bBox.intersectsRay(r)) {
    return false;
  }

  Intersection closest_hit = record;
  bool hit_found = false;
  if (isWorld()) {
    for (const auto& child : getChildren()) {
      if (child->intersect(r, closest_hit)) {
        hit_found = true;
        if (closest_hit.thit < record.thit) {
          record = closest_hit;
        }
      }
    }
  } else {
    if (m_geometric_primitive->intersect(r, closest_hit) &&
        (closest_hit.thit >= r.getMinRange() &&
         closest_hit.thit < r.getMaxRange())) {
      hit_found = true;
      if (closest_hit.thit < record.thit) {
        closest_hit.closest_scene_element = this;
        record = closest_hit;
      }
    }
  }
  return hit_found;
}

void SceneElementNode::add(SceneElementPtr item) {
  item->setParent(this);
  m_bBox.addBox(item->getBoundingBox());
  m_children.emplace_back(item);
}

bool SceneElementNode::isWorld() const { return !m_children.empty(); }

void SceneElementNode::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElement(this, ray);
}

std::vector<std::shared_ptr<SceneElementNode>> SceneElementNode::getChildren()
    const {
  return m_children;
}

std::vector<std::shared_ptr<SceneElementNode>>&
SceneElementNode::getChildren() {
  return m_children;
}

void SceneElementNode::setMaterial(MaterialPtr mat) { m_material = mat; }

const MaterialRawPtr SceneElementNode::getMaterial() const {
  return m_material.get();
}

void SceneElementNode::setPrimitive(PrimitivePtr pr) {
  m_geometric_primitive = pr;
  m_bBox = m_geometric_primitive->worldBounds();
}

PrimitivePtr SceneElementNode::getPrimitive() { return m_geometric_primitive; }

std::shared_ptr<SceneElementNode> SceneElementNode::create() {
  return std::make_shared<SceneElementNode>();
}

void SceneElementNode::setParent(SceneElementRawPtr parent) {
  m_parent = parent;
}

SceneElementRawPtr SceneElementNode::getParent() const { return m_parent; }

void SceneElementNode::setLight(const PointLight& light) { m_light = light; }

PointLight SceneElementNode::getLight() const { return m_light; }

BoundingBox SceneElementNode::getBoundingBox() const { return m_bBox; }
