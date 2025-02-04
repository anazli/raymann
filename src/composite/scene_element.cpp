#include "composite/scene_element.h"

#include "renderers/renderer.h"
#include "world.h"

// size_t SceneElement::m_next_id = 0;

SceneElementType SceneElement::elementType() const { return m_elementType; }

bool SceneElement::intersect(const Ray& r, IntersectionRecord& record) {
  return false;
}

void SceneElement::add(SceneElementPtr item) {}

SceneElementContainer::iterator SceneElement::remove(
    SceneElementRawPtr item, SceneElementPtr removedElem) {}

bool SceneElement::isWorld() const { return false; }

Vec3f SceneElement::normal(const Point3f& p) const { return Vec3f(); }

void SceneElement::accept(BaseRenderer& renderer, const Ray& ray) {
  renderer.visitSceneElementLeaf(this, ray);
}

SceneElementContainer SceneElement::getChildren() const {
  return SceneElementContainer();
}

std::vector<std::shared_ptr<SceneElement> >& SceneElement::getChildren() {}

void SceneElement::setMaterial(BaseMaterialPtr mat) { m_material = mat; }

BaseMaterialPtr SceneElement::getMaterial() const { return m_material; }

void SceneElement::setParent(SceneElementRawPtr parent) { m_parent = parent; }

SceneElementRawPtr SceneElement::getParent() const { return m_parent; }

void SceneElement::setLight(const PointLight& light) {}

PointLight SceneElement::getLight() const { return PointLight(); }

void SceneElement::setBoundingBox(const BoundingBox& box) { m_bBox = box; }

BoundingBox& SceneElement::boundingBox() { return m_bBox; }

const BoundingBox& SceneElement::boundingBox() const { return m_bBox; }

float SceneElement::pdf(const Point3f& origin, const Vec3f& direction) {
  return 1.f;
}

Vec3f SceneElement::random(const Point3f& origin) {
  return Vec3f(1.f, 0.f, 0.f);
}

// size_t SceneElement::getId() const { return 0; /*m_id*/ }

Point3f SceneElement::pointFromWorldToObjectSpace(const Point3f& point) const {
  return Mat4f().inverse() * Vec4f(point);
}

Vec3f SceneElement::vectorFromObjectToWorldSpace(const Vec3f vec) const {
  Vec3f v(vec);
  v = Mat4f().inverse().transpose() * Vec4f(v);
  v.normalize();
  return v;
}

SceneElement::SceneElement(const BoundingBox& props) : m_bBox(props) {}

/*SceneElement::SceneElement() {
  m_id = m_next_id;
  m_next_id++;
}*/
