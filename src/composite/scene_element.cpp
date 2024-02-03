#include "composite/scene_element.h"

#include "renderers/renderer.h"

BoundingBoxProperties::BoundingBoxProperties(const Point3f& pmin,
                                             const Point3f& pmax)
    : m_minPoint(pmin), m_maxPoint(pmax) {}

Point3f& BoundingBoxProperties::minPoint() { return m_minPoint; }

Point3f& BoundingBoxProperties::maxPoint() { return m_maxPoint; }

const Point3f& BoundingBoxProperties::minPoint() const { return m_minPoint; }

const Point3f& BoundingBoxProperties::maxPoint() const { return m_maxPoint; }

// size_t SceneElement::m_next_id = 0;

SceneElementType SceneElement::elementType() const { return m_elementType; }

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

std::list<SceneElementPtr> SceneElement::getChildren() {
  return std::list<SceneElementPtr>();
}

void SceneElement::setMaterial(BaseMaterialPtr mat) { m_material = mat; }

BaseMaterialPtr SceneElement::getMaterial() const { return m_material; }

Mat4f SceneElement::transformationMatrix() const { return Mat4f(); }

void SceneElement::setParent(SceneElementRawPtr parent) { m_parent = parent; }

SceneElementRawPtr SceneElement::getParent() const { return m_parent; }

void SceneElement::setLight(const PointLight& light) {}

PointLight SceneElement::getLight() const { return PointLight(); }

void SceneElement::addPoint(const Point3f& point) {}

bool SceneElement::containsPoint(const Point3f& point) const {
  // three-way comparison doesn't work
  return point.x() >= m_bBoxProps.minPoint().x() &&
         point.x() <= m_bBoxProps.maxPoint().x() &&

         point.y() >= m_bBoxProps.minPoint().y() &&
         point.y() <= m_bBoxProps.maxPoint().y() &&

         point.z() >= m_bBoxProps.minPoint().z() &&
         point.z() <= m_bBoxProps.maxPoint().z();
}

bool SceneElement::containsBoundingBox(
    const BoundingBoxProperties& prop) const {
  return containsPoint(prop.minPoint()) && containsPoint(prop.maxPoint());
}

void SceneElement::setBoundingBoxProperties(
    const BoundingBoxProperties& props) {
  m_bBoxProps = props;
}

BoundingBoxProperties& SceneElement::boundingBoxProperties() {
  return m_bBoxProps;
}

const BoundingBoxProperties& SceneElement::boundingBoxProperties() const {
  return m_bBoxProps;
}

// size_t SceneElement::getId() const { return 0; /*m_id*/ }

Point3f SceneElement::pointFromWorldToObjectSpace(const Point3f& point) const {
  return Mat4f().inverse() * Vec4f(point);
}

Vec3f SceneElement::vectorFromObjectToWorldSpace(const Vec3f vec) const {
  Vec3f v(vec);
  v = Mat4f().inverse().transpose() * Vec4f(v);
  return v.normalize();
}

SceneElement::SceneElement(const BoundingBoxProperties& props)
    : m_bBoxProps(props) {}

/*SceneElement::SceneElement() {
  m_id = m_next_id;
  m_next_id++;
}*/
