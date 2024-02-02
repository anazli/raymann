#include "acceleration/scene_element_proxy.h"

SceneElementProxy::SceneElementProxy(SceneElementPtr element,
                                     const Point3f &minPoint,
                                     const Point3f &maxPoint)
    : m_sceneElement(element),
      SceneElement(BoundingBoxProperties(minPoint, maxPoint)) {}

bool SceneElementProxy::intersect(const Ray &r, IntersectionRecord &record) {
  return false;
}

void SceneElementProxy::add(SceneElementPtr item) {
  if (m_sceneElement->isWorld()) {
    addPoint(item->boundingBoxProperties().minPoint());
    addPoint(item->boundingBoxProperties().maxPoint());
    if (item->containsElement()) {
      item->getElementOfBoundingBox()->setParent(m_sceneElement.get());
      m_sceneElement->add(item->getElementOfBoundingBox());
    } else {
      item->setParent(m_sceneElement.get());
      m_sceneElement->add(item);
    }
  }
}

bool SceneElementProxy::containsElement() const {
  return m_sceneElement != nullptr;
}

SceneElementPtr SceneElementProxy::getElementOfBoundingBox() const {
  return m_sceneElement;
}

void SceneElementProxy::setElementOfBoundingBox(SceneElementPtr element) {
  m_sceneElement = element;
}

void SceneElementProxy::addPoint(const Point3f &point) {
  if (point.x() < m_bBoxProps.minPoint().x())
    m_bBoxProps.minPoint().setX(point.x());
  if (point.y() < m_bBoxProps.minPoint().y())
    m_bBoxProps.minPoint().setY(point.y());
  if (point.z() < m_bBoxProps.minPoint().z())
    m_bBoxProps.minPoint().setZ(point.z());

  if (point.x() > m_bBoxProps.maxPoint().x())
    m_bBoxProps.maxPoint().setX(point.x());
  if (point.y() > m_bBoxProps.maxPoint().y())
    m_bBoxProps.maxPoint().setY(point.y());
  if (point.z() > m_bBoxProps.maxPoint().z())
    m_bBoxProps.maxPoint().setZ(point.z());
}

bool SceneElementProxy::containsPoint(const Point3f &point) const {
  // three-way comparison doesn't work
  return point.x() >= m_bBoxProps.minPoint().x() &&
         point.x() <= m_bBoxProps.maxPoint().x() &&

         point.y() >= m_bBoxProps.minPoint().y() &&
         point.y() <= m_bBoxProps.maxPoint().y() &&

         point.z() >= m_bBoxProps.minPoint().z() &&
         point.z() <= m_bBoxProps.maxPoint().z();
}

bool SceneElementProxy::containsBoundingBox(
    const BoundingBoxProperties &prop) const {
  return containsPoint(prop.minPoint()) && containsPoint(prop.maxPoint());
}
