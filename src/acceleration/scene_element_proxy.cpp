#include "acceleration/scene_element_proxy.h"

SceneElementProxy::SceneElementProxy(SceneElementPtr element,
                                     const Point3f &minPoint,
                                     const Point3f &maxPoint)
    : m_sceneElement(element),
      SceneElement(BoundingBoxProperties(minPoint, maxPoint)) {}

bool SceneElementProxy::intersect(const Ray &r, IntersectionRecord &record) {
  return false;
}

// SceneElementPtr SceneElementProxy::getSceneElement() const {
//   return m_sceneElement;
// }

// void SceneElementProxy::setSceneElement(SceneElementPtr element) {
//   m_sceneElement = element;
// }

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
