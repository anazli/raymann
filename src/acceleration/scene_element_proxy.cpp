#include "acceleration/scene_element_proxy.h"

#include "application/error.h"

SceneElementProxy::SceneElementProxy(SceneElementPtr element)
    : m_sceneElement(element) {
  if (m_sceneElement && m_sceneElement->transformationMatrix() != Mat4f()) {
    transformBox();
  } else if (m_sceneElement) {
    m_bBoxProps = m_sceneElement->boundingBoxProperties();
  }
}

SceneElementProxy::SceneElementProxy(SceneElementPtr element,
                                     const Point3f &minPoint,
                                     const Point3f &maxPoint)
    : m_sceneElement(element),
      SceneElement(BoundingBoxProperties(minPoint, maxPoint)) {
  if (m_sceneElement && m_sceneElement->transformationMatrix() != Mat4f()) {
    transformBox();
  }
}

bool SceneElementProxy::intersect(const Ray &r, IntersectionRecord &record) {
  return false;
}

void SceneElementProxy::add(SceneElementPtr item) {
  APP_ASSERT(item, "Item is null");
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

void SceneElementProxy::transformBox() {
  Point3f p1 = m_bBoxProps.minPoint();
  Point3f p2 = Point3f(m_bBoxProps.minPoint().x(), m_bBoxProps.minPoint().y(),
                       m_bBoxProps.maxPoint().z());
  Point3f p3 = Point3f(m_bBoxProps.minPoint().x(), m_bBoxProps.maxPoint().y(),
                       m_bBoxProps.minPoint().z());
  Point3f p4 = Point3f(m_bBoxProps.minPoint().x(), m_bBoxProps.maxPoint().y(),
                       m_bBoxProps.maxPoint().z());
  Point3f p5 = Point3f(m_bBoxProps.maxPoint().x(), m_bBoxProps.minPoint().y(),
                       m_bBoxProps.minPoint().z());
  Point3f p6 = Point3f(m_bBoxProps.maxPoint().x(), m_bBoxProps.minPoint().y(),
                       m_bBoxProps.maxPoint().z());
  Point3f p7 = Point3f(m_bBoxProps.maxPoint().x(), m_bBoxProps.maxPoint().y(),
                       m_bBoxProps.minPoint().z());
  Point3f p8 = m_bBoxProps.maxPoint();
  m_bBoxProps = BoundingBoxProperties{};
  std::vector<Point3f> v{p1, p2, p3, p4, p5, p6, p7, p8};
  for (Point3f &elem : v) {
    addPoint(m_sceneElement->transformationMatrix() * Vec4f(elem));
  }
}
