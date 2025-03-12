#include "composite/scene_element.h"

#include "iterator.h"
#include "renderers/renderer.h"
#include "scene_element.h"

SceneElement::SceneElement() {}

bool SceneElement::intersect(const Ray& r, Intersection& record) {
  if (!m_bBox.intersectsRay(r)) {
    return false;
  }
  auto hit_found = false;
  if (isWorld()) {
    for (const auto& child : getChildren()) {
      if (child->intersect(r, record)) {
        return true;
      }
    }
  } else {
    auto rec = Intersection();
    if (m_geometric_primitive->intersect(r, rec) &&
        (rec.min_hit >= r.getMinRange() && rec.min_hit < r.getMaxRange())) {
      hit_found = true;
      if (rec.min_hit < record.min_hit) {
        rec.primitive = this;
        rec.min_hit = rec.min_hit;
        record = rec;
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

std::shared_ptr<SceneElement> SceneElement::create() {
  return std::make_shared<SceneElement>();
}

void SceneElement::setParent(SceneElementRawPtr parent) { m_parent = parent; }

SceneElementRawPtr SceneElement::getParent() const { return m_parent; }

void SceneElement::setLight(const PointLight& light) { m_light = light; }

PointLight SceneElement::getLight() const { return m_light; }

BoundingBox SceneElement::getBoundingBox() const { return m_bBox; }
