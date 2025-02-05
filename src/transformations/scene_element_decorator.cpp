#include "transformations/scene_element_decorator.h"

SceneElementDecorator::SceneElementDecorator(SceneElementRawPtr tr,
                                             const Mat4D& m)
    : m_element(tr), m_transformMatrix(m) {}

SceneElementDecorator::~SceneElementDecorator() { delete m_element; }

SceneElementType SceneElementDecorator::elementType() const {
  return m_element->elementType();
}

bool SceneElementDecorator::intersect(const Ray& r,
                                      IntersectionRecord& record) {
  return m_element->intersect(r, record);
}

Vec3D SceneElementDecorator::normal(const Point3D& p) const {
  return m_element->normal(p);
}

void SceneElementDecorator::add(SceneElementPtr item) { m_element->add(item); }

SceneElementContainer::iterator SceneElementDecorator::remove(
    SceneElementRawPtr item, SceneElementPtr removedElem) {
  return m_element->remove(item, removedElem);
}

bool SceneElementDecorator::isWorld() const { return m_element->isWorld(); }

void SceneElementDecorator::accept(BaseRenderer& renderer, const Ray& ray) {
  m_element->accept(renderer, ray);
}

SceneElementContainer SceneElementDecorator::getChildren() const {
  return m_element->getChildren();
}

SceneElementContainer& SceneElementDecorator::getChildren() {
  return m_element->getChildren();
}

void SceneElementDecorator::setParent(SceneElementRawPtr parent) {
  m_element->setParent(parent);
}

SceneElementRawPtr SceneElementDecorator::getParent() const {
  return m_element->getParent();
}

void SceneElementDecorator::setBoundingBox(const BoundingBox& box) {
  m_element->setBoundingBox(box);
}

BoundingBox& SceneElementDecorator::boundingBox() {
  return m_element->boundingBox();
}

const BoundingBox& SceneElementDecorator::boundingBox() const {
  return m_element->boundingBox();
}

float SceneElementDecorator::pdf(const Point3D& origin,
                                 const Vec3D& direction) {
  return m_element->pdf(origin, direction);
}

Vec3D SceneElementDecorator::random(const Point3D& origin) {
  return m_element->random(origin);
}

Point3D SceneElementDecorator::pointFromWorldToObjectSpace(
    const Point3D& point) const {
  return m_element->pointFromWorldToObjectSpace(point);
}

Vec3D SceneElementDecorator::vectorFromObjectToWorldSpace(
    const Vec3D vec) const {
  return m_element->vectorFromObjectToWorldSpace(vec);
}
